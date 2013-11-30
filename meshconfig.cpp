// Copyright Vladimir Prus 2002-2004.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

/* Shows how to use both command line and config file. */

#include <boost/program_options.hpp>
namespace po = boost::program_options;


#include <iostream>
#include <fstream>
#include <iterator>
using namespace std;

// A helper function to simplify the main part.
template<class T>
ostream& operator<<(ostream& os, const vector<T>& v)
{
    copy(v.begin(), v.end(), ostream_iterator<T>(os, " ")); 
    return os;
}

int getmeshconfig(int ac, char * av[],int& nR, int& nZ, int& nPhi);


int main(int ac, char* av[])
{
	int nR=1,nZ=1,nPhi=1;
	getmeshconfig(ac, av, nR, nZ, nPhi);
        cout << "nR: " << nR<<", nZ: "<<nZ<<", nPhi: "<<nPhi << "\n";  
	return 0;
}

int getmeshconfig(int ac, char * av[],int& nR, int& nZ, int& nPhi)
{
    try {
        string config_file;
    
        // Declare a group of options that will be 
        // allowed only on command line
        po::options_description generic("Generic options");
        generic.add_options()
            ("version,v", "print version string")
            ("help", "produce help message")
            ("config,c", po::value<string>(&config_file)->default_value("mesh_config.cfg"),
                  "name of a file of a configuration.")
            ;
    
        // Declare a group of options that will be 
        // allowed both on command line and in
        // config file
        po::options_description config("Configuration");
        config.add_options()
            ("nR", po::value<int>(&nR)->default_value(10), 
                  "Number of R grid")
            ("nZ", po::value<int>(&nZ)->default_value(10), 
                  "Number of Z grid")
            ("nPhi", po::value<int>(&nPhi)->default_value(10), 
                  "Number of Phi grid")
            ;

        // Hidden options, will be allowed both on command line and
        // in config file, but will not be shown to the user.
        po::options_description hidden("Hidden options");
        hidden.add_options()
            ("input-file", po::value< vector<string> >(), "input file")
            ;

        
        po::options_description cmdline_options;
        cmdline_options.add(generic).add(config).add(hidden);

        po::options_description config_file_options;
        config_file_options.add(config).add(hidden);

        po::options_description visible("Allowed options");
        visible.add(generic).add(config);
        
        po::positional_options_description p;
        p.add("input-file", -1);
        
        po::variables_map vm;
        store(po::command_line_parser(ac, av).
              options(cmdline_options).positional(p).run(), vm);
        notify(vm);
        
        ifstream ifs(config_file.c_str());
        if (!ifs)
        {
            cout << "can not open config file: " << config_file << "\n";
            return 0;
        }
        else
        {
            store(parse_config_file(ifs, config_file_options), vm);
            notify(vm);
        }
    
        if (vm.count("help")) {
            cout << visible << "\n";
            return 0;
        }

        if (vm.count("version")) {
            cout << "Multiple sources example, version 1.0\n";
            return 0;
        }

              
    }
    catch(exception& e)
    {
        cout << e.what() << "\n";
        return 1;
    }    
}
