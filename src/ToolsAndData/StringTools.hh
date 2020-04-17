/***********************************************************************
 *  This code is part of MpsSolver
 *
 *  Copyright (C) Mike Steglich - Technical University of Applied Sciences
 *  Wildau, Germany 
 *
 *  MpsSolver is free software; you can redistribute it and/or modify it 
 *  under the terms of the GNU General Public License as published by 
 *  the Free Software Foundation; either version 3 of the License, or 
 *  (at your option) any later version.
 * 
 *  MpsSolver is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public 
 *  License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 ***********************************************************************/

#ifndef STRINGTOOLS_HH
#define STRINGTOOLS_HH

//#include <string.h>
#include <string>
#include <vector>

using namespace std;



/**
	 * Tools for string handling
	 */
class StringTools
{

public:
     /**
     * constructor
     */
     StringTools();

     /**
     * destructor
     */
     ~StringTools();

/*********** static string utility functions ********************/

// white space characters
#define WHITE_SPACES " \t\f\v\n\r"

     /**
     * @brief returns a left trimmed string
     * @param str		string to trim
     * @param ws		space chars
     * @return			left trimmed string
     */
     static string lTrim(const string &str, const char *ws = WHITE_SPACES);

     /**
     * @brief returns a right trimmed string
     * @param str		string to trim
     * @param ws		space chars
     * @return			right trimmed string
     */
     static string rTrim(const string &str, const char *ws = WHITE_SPACES);

     /**
     * @brief returns a left and right trimmed string
     * @param str		string to trim
     * @param ws		space chars
     * @return			left and right trimmed string
     */
     static string lrTrim(const string &str, const char *ws = WHITE_SPACES);

     /**
     * @brief returns string in upper cases
     * @param str		string to transform to upper case
     * @return			string in upper case
     */
     static string upperCase(const string &str);

     /**
     * @brief returns string in lower cases
     * @param str		string to transform to lower case
     * @return			string in lower case
     */
     static string lowerCase(const string &str);


     /**
     * @brief replaces in a string a substring with another substring
     * @param str          String in that substrings have to be exchanged
     * @param targ         substring to be exchanged
     * @param repl         substring to exchange the targ substring
     * @return
     */
     static string replaceString(const string &str, const string &targ, const string &repl);

     /**
     * @brief returns whether a string starts with a particular string
     * @param str		string to check
     * @param start     start string
     * @return			true or false
     */
     static bool startsWith(const string &str, const string &start);


     /**
     * @brief returns whether a string ends with a particular string
     * @param str		string to check
     * @param end       start string
     * @return			true or false
     */
     static bool endsWith(string str, string end);


     /**
     * @brief returns whether a string contains a particular string
     * @param str		string to check
     * @param str1      string to be checked as part of str
     * @return			true or false
     */
     static bool contains(const string &str, const string &str1);


     /**
       * @brief Splits a whitespace separated option into a key and a value
       * @param str          String to be splitted into a key and a value
       * @param key          key of the option
       * @param val          value of the option
      */
     static void getOptionValue(const string &str, string &key, string &val);

     /**
       * @brief Splits an option into a key and a value
       * @param str          String to be split into a key and a value
       * @param key          key of the option
       * @param val          value of the option
       * @param sep          separator char
      */
     static void getOptionValue(const string &str, string &key, string &val, const char *sep);

     /**
       * @brief splits a string into a string vector using a particular separator
       * @param str          String to be split
       * @param strVec       String vector for the entries
       * @param sep          separator
      */
     static void split(const string &str, vector<string> &strVec, const char *sep = WHITE_SPACES);

     /**
       * @brief Converts a string into int
       * @param str          String to be converted
       * @param t            int result
       * @return             true if convertion is successfull
      */
     static bool toInt(const string &str, int &t);

     /**
       * @brief Converts a string into long
       * @param str          String to be converted
       * @param t            long result
       * @return             true if convertion is successfull
      */
     static bool toLong(const string &str, long &t);

     /**
       * @brief Converts a string into double
       * @param str          String to be converted
       * @param t            double result
       * @return             true if convertion is successfull
      */
     static bool toDouble(const string &str, double &t);

     /**
       * @brief Converts a string into double
       * @param str          String to be converted
       * @param t            double result
       * @param prec         zero precision
       * @return             true if convertion is successfull
      */
     static bool toDouble(const string &str, double &t, const double prec);

     /**
      * @brief   If a string is enclosed in double quotes then the doubles quotes will be cleaned.
      * @param str          String to be cleaned
     */
     static void cleanFromDoubleQuotes(string &str);

     /**
      * Cleans a string from a specific string
      */

     /**
      * @brief   Cleans a string from a specific string
      * @param str          String to be cleaned
      * @param str1         substring to be cleaned in str
     */
     static string cleanFromString(const string &str, const string &str1);

     /**
          * encodes xml expressions in a string
          * @param           string to be encoded
          * @return			encoded string
         */
     static string encodeXml(string &str);

     /**
     * @brief  iterates over words in a string, on each call the next word is given back
     * @param str		string to iterate over
     * @param word		return of next word
     * @param pos		current position in string, start iteration with 0
     * @param ws		separating chars
     * @return			false if no more word found
		 */
     static bool iterWords(const string &str, string &word, size_t &pos, const char *sep = WHITE_SPACES)
     {
          bool quote;
          return iterWords(str, word, pos, quote, '\0', sep);
     }

     /**
     * @brief  iterates over words in a string, on each call the next word is given back, with regarding of quotes
     * @param str		string to iterate over
     * @param word		return of next word
     * @param pos		current position in string, start iteration with 0
     * @param quote		return if the word was quoted
     * @param qc		quote char / '\0' no quote char
     * @param ws		separating chars
     * @return			false if no more word found
		 */
     static bool iterWords(const string &str, string &word, size_t &pos, bool &quote, char qc = '"', const char *sep = WHITE_SPACES);

     /**
      * @brief cleans a string from all witeSpaces
      * @param str      string to be cleaned
      */
     static void cleanFromWhiteSpaces(string &str);

     /**
     * @brief returns a problem name w/o the extension .mps
     * @param probName
     * @return problem name
     */
     static string problemName(const string &probName);

     /**
     * @brief returns the path of a problem
     *
     * @param probName
     * @return problem path
     */
     static string problemPath(const string &probName);

     /**
     * @brief returns the name of a model
     *
     * @param probName
     * @return model name w/o extension
     */
     static string modelName(const string &probName);


     /**
      * @brief Returns true if a string was found in a vector of strings
      * @param vec      vector of strings
      * @param str      string to be found in vec
      * @return         true if str was found in vec
      */
     static bool findInStringVector(vector <string> &vec, const string  &str);

};


#endif // !STRINGTOOLS_HH
