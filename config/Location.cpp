#include "Location.hpp"

Location::Location() : path( "" ), root( "" ), index( "" ), autoindex( false ), cgi( "" ), upload_path( "" ), redirect( "" ) {
	methods.clear();
}

Location::Location( string locationPath ) {
	path = locationPath;
}

void Location::setSingDirectives( string key, vector< string > values ) {
	if ( values.size() != 1 )
		throw runtime_error( "Error: One '" + key + "' in 'Location' Required" );

	string value = values[0];
	if ( key == "root" )
		root = value;
	else if ( key == "index" )
		index = value;
	else if ( key == "autoindex" ) {
		if ( value != "on" && value != "off" )
			throw runtime_error( "Error: autoindex must have 'on' or 'off' value" );
		autoindex = ( value == "on" );
	} else if ( key == "upload_path" )
		upload_path = value;
	else if ( key == "redirect" )
		redirect = value;
}

void Location::setMethods( vector< string > values ) {
	string			 locationMethods[] = { "GET", "POST", "DELETE" };
	vector< string > validMethods( locationMethods, locationMethods + 3 );
	for ( size_t i = 0; i < values.size(); i++ ) {
		if ( find( validMethods.begin(), validMethods.end(), values[i] ) == validMethods.end() )
			throw runtime_error( "Error: Invalid method '" + values[i] + "'" );

		methods.insert( values[i] );
	}
}

void Location::setDirectives( map< string, vector< string > > directives ) {
	string			 tokens[] = { "root", "index", "autoindex", "cgi", "upload_path", "redirect", "methods" };
	vector< string > locationTokens( tokens, tokens + 7 );

	for ( map< string, vector< string > >::iterator iter = directives.begin(); iter != directives.end(); iter++ ) {
		if ( iter->first == "methods" )
			setMethods( iter->second );
		else if ( find( locationTokens.begin(), locationTokens.end(), iter->first ) != locationTokens.end() )
			setSingDirectives( iter->first, iter->second );
		else
			throw runtime_error( "Error: Invalid token '" + iter->first + "' in Location" );
	}
}

// Getters
string Location::getPath() const {
	return path;
}

string Location::getRoot() const {
	return root;
}

string Location::getIndex() const {
	return index;
}

bool Location::getAutoindex() const {
	return autoindex;
}

string Location::getCgi() const {
	return cgi;
}

string Location::getUploadPath() const {
	return upload_path;
}

string Location::getRedirect() const {
	return redirect;
}

set< string > Location::getMethods() const {
	return methods;
}

// Setters
void Location::setPath( const string& path ) {
	this->path = path;
}

void Location::setRoot( const string& root ) {
	this->root = root;
}

void Location::setIndex( const string& index ) {
	this->index = index;
}

void Location::setAutoindex( bool autoindex ) {
	this->autoindex = autoindex;
}

void Location::setCgi( const string& cgi ) {
	this->cgi = cgi;
}

void Location::setUploadPath( const string& uploadPath ) {
	this->upload_path = uploadPath;
}

void Location::setRedirect( const string& redirect ) {
	this->redirect = redirect;
}

void Location::setMethods( const set< string >& methods ) {
	this->methods = methods;
}