#include "gigademos.h"

#include "core/interfaces/interfaces.h"
#include "sdk/photon.h"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <functional>
#include <map>
#include <thread>
#include <vector>
#include <zip.h>

static std::thread worker;

static zip_t* zip = nullptr;

static void play_next_demo( ) {
	static ssize_t i{ };
	static ssize_t max = zip_entries_total( zip );

	char*  buf;
	size_t bufsize;

	// read entry into buffer.
	zip_entry_openbyindex( zip, i );
	zip_entry_read( zip, ( void** ) &buf, &bufsize );
	zip_entry_close( zip );

	std::ofstream o{ "portal2/temp.dem", std::ios_base::binary };
	o.write( buf, bufsize );
	o.close( );

	interfaces::engine_client->execute_client_cmd( "playdemo temp" );

	++i;

	if ( i == max ) {
		zip_close( zip );
		zip = nullptr;
	}
}

// TODO: move.
SIGNAL_CALLBACK( void, __rescall, frame ) {
	original( ecx );

	if ( zip && !interfaces::demo_player->is_playing_back( ) ) {
		play_next_demo( );
	}
}

template < typename t >
time_t to_time_t( t tp ) {
	using namespace std::chrono;
	auto sctp = time_point_cast< system_clock::duration >( tp - t::clock::now( ) + system_clock::now( ) );
	return system_clock::to_time_t( sctp );
}

static void pack_gdem( ) {
	// store in map that is sorted by write time.
	std::map< time_t, std::filesystem::directory_entry > entries;
	for ( const auto& entry : std::filesystem::directory_iterator( "gdem" ) ) {
		if ( entry.is_regular_file( ) ) {
			auto time       = to_time_t( entry.last_write_time( ) );
			entries[ time ] = entry;
		}
	}

	auto iter = std::filesystem::directory_iterator( "gdem" );

	zip_t* zip = zip_open( "gdem/example.gdem", ZIP_DEFAULT_COMPRESSION_LEVEL, 'w' );
	for ( const auto& [ time, entry ] : entries ) {
		zip_entry_open( zip, entry.path( ).stem( ).string( ).c_str( ) );

		// read bytes from file.
		std::ifstream i{ entry.path( ), std::ios_base::binary };

		std::vector< char > bytes( entry.file_size( ) );
		i.read( bytes.data( ), entry.file_size( ) );

		// write bytes to zip.
		zip_entry_write( zip, bytes.data( ), bytes.size( ) );
		zip_entry_close( zip );
	}
	zip_close( zip );
}

static void pack_gdem_cbk( const c_command& args ) {
	if ( worker.joinable( ) )
		worker.join( );

	worker = std::thread( pack_gdem );
}

static void play_gdem_cbk( const c_command& args ) {
	zip = zip_open( "gdem/example.gdem", 0, 'r' );

	play_next_demo( );
}

bool gigademos::initialize( ) {
	photon->signal->get( "frame" )->add_callback( &frame_cbk );

	photon->con->create_concmd( "pack_gdem", pack_gdem_cbk );
	photon->con->create_concmd( "play_gdem", play_gdem_cbk );

	return true;
}
void gigademos::uninitialize( ) {
	photon->con->destruct_concmd( "play_gdem" );
	photon->con->destruct_concmd( "pack_gdem" );

	photon->signal->get( "frame" )->remove_callback( &frame_cbk );
}
