#pragma once

#include "../srcs/bits.hpp"

class Location {
	private:
		string		  path;
		string		  root;
		string		  index;
		bool		  autoindex;
		string		  cgi;
		string		  upload_path;
		string		  redirect;
		set< string > methods;

	public:
		Location();
		Location( string locationPath );
		void setDirectives( map< string, vector< string > > directives );
		void setSingDirectives( string key, vector< string > values );
		void setMethods( vector< string > values );

		// Getters
		string getPath() const;
		string getRoot() const;
		string getIndex() const;
		bool getAutoindex() const;
		string getCgi() const;
		string getUploadPath() const;
		string getRedirect() const;
		set< string > getMethods() const;

		// Setters
		void setPath(const string& path);
		void setRoot(const string& root);
		void setIndex(const string& index);
		void setAutoindex(bool autoindex);
		void setCgi(const string& cgi);
		void setUploadPath(const string& uploadPath);
		void setRedirect(const string& redirect);
		void setMethods(const set< string >& methods);
};
