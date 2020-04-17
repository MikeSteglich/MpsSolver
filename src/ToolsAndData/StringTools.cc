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

#include "StringTools.hh"
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>
#include <cmath>

/**
     * constructor
     */
StringTools::StringTools()
{
}

/**
     * destructor
     */
StringTools::~StringTools()
{
}


/**
    * @brief returns a left trimmed string
    * @param str		string to trim
    * @param ws		space chars
    * @return			left trimmed string
    */
string StringTools::lTrim(const string &str, const char *ws )
{
    size_t p = str.find_first_not_of(ws);
    if (p == string::npos)
        return "";
    else
        return str.substr(p);
}


/**
    * @brief returns a right trimmed string
    * @param str		string to trim
    * @param ws		space chars
    * @return			right trimmed string
    */
string StringTools::rTrim(const string &str, const char *ws )
{
    size_t p = str.find_last_not_of(ws);
    if (p == string::npos)
        return "";
    else
        return str.substr(0, p + 1);
}

/**
    * @brief returns a left and right trimmed string
    * @param str		string to trim
    * @param ws		space chars
    * @return			left and right trimmed string
    */
string StringTools::lrTrim(const string &str, const char *ws )
{
    size_t p = str.find_first_not_of(ws);
    if (p == string::npos)
        return "";
    else
        return str.substr(p, str.find_last_not_of(ws) - p + 1);
}

/**
    * @brief returns string in upper cases
    * @param str		string to transform to upper case
    * @return			string in upper case
    */
string StringTools::upperCase(const string &str)
{
    string tmpString = str;
    transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::toupper);
    return tmpString;
}

/**
    * @brief returns string in lower cases
    * @param str		string to transform to lower case
    * @return			string in lower case
    */
string StringTools::lowerCase(const string &str)
{
    string tmpString = str;
    transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);
    return tmpString;
}

/**
 * @brief replaces in a string a substring with another substring
 * @param str          String in that substrings have to be exchanged
 * @param targ         substring to be exchanged
 * @param repl         substring to exchange the targ substring
 * @return
 */
string StringTools::replaceString(const string &str, const string &targ, const string &repl)
{
    string tmpStr = str;
    if (targ.empty() || repl.empty())
        return tmpStr;

    size_t idx = 0;
    while (true)
    {
        idx = tmpStr.find(targ, idx);
        if (idx == string::npos)
            break;

        tmpStr.replace(idx, targ.length(), repl);

        idx += repl.length();
    }
    return tmpStr;
} // end replaceString


/**
    * @brief returns whether a string starts with a particular string
    * @param str		string to check
    * @param start      start string
    * @return			true or false
    */
bool StringTools::startsWith(const string &str, const string &start)
{
    return (str.substr(0, start.length()) == start);
}


/**
    * @brief returns whether a string ends with a particular string
    * @param str		string to check
    * @param end      start string
    * @return			true or false
    */
bool StringTools::endsWith(string str, string end)
{
    if (str.length() < end.length())
        return false;
    else
        return (str.substr(str.length() - end.length()) == end);
} // end stringEndsWith


/**
    * @brief returns whether a string contains a particular string
    * @param str		string to check
    * @param str1      string to be checked as part of str
    * @return			true or false
    */
bool StringTools::contains(const string &str, const string &str1)
{
    return (str.find(str1) != string::npos);
}

/**
 * @brief Splits a whitespace separated option into a key and a value
 * @param str          String to be splitted into a key and a value
 * @param key          key of the option
 * @param val          value of the option
 */
void StringTools::getOptionValue(const string &str, string &key, string &val )
{
    istringstream iss(str);

    iss >> key;
    iss >> val;

    key = StringTools::lrTrim(key);
    val = StringTools::lrTrim(val);
}

/**
 * @brief Splits an option into a key and a value
 * @param str          String to be splitted into a key and a value
 * @param key          key of the option
 * @param val          value of the option
 * @param sep          separator char
 */
void StringTools::getOptionValue(const string &str, string &key, string &val, const char *sep)
{
    vector<string> opt(0);
    split(str, opt, sep);

    key = opt[0];
    if (opt.size()>1  )
        val = opt[1];
    else
        val = "";
}

/**
 * @brief splits a string into a string vector using a particular separator
 * @param str          String to be split
 * @param strVec       String vector for the entries
 * @param sep          separator
 */
void StringTools::split(const string &str, vector<string> &strVec, const char *sep)
{

    string entry;
    size_t pn = 0;

    while (iterWords(str, entry, pn, sep))
    {
        entry = StringTools::lrTrim(entry);
        strVec.push_back(entry);
    }
}

/**
 * @brief Converts a string into int
 * @param str          String to be converted
 * @param t            int result
 * @return             true if convertion is successfull
 */
bool StringTools::toInt(const string &str, int &t)
{
    string tmpStr = StringTools::lrTrim(str);
    if (tmpStr.find_first_not_of("0123456789-e") != string::npos)
        return false;
    stringstream sStr(tmpStr);
    return !((sStr >> t).fail());
}

/**
 * @brief Converts a string into long
 * @param str          String to be converted
 * @param t            long result
 * @return             true if convertion is successfull
 */
bool StringTools::toLong(const string &str, long &t)
{
    string tmpStr = StringTools::lrTrim(str);
    if (tmpStr.find_first_not_of("0123456789-e") != string::npos)
        return false;
    stringstream sStr(tmpStr);
    return !((sStr >> t).fail());
}


/**
 * @brief Converts a string into double
 * @param str          String to be converted
 * @param t            double result
 * @return             true if convertion is successfull
 */
bool StringTools::toDouble(const string &str, double &t)
{
    string tmpStr = StringTools::lrTrim(str);
    if (tmpStr.find_first_not_of("0123456789.-+e") != string::npos)
        return false;
    stringstream sStr(tmpStr);
    return !((sStr >> t).fail());
}


/**
  * @brief Converts a string into double
  * @param str          String to be converted
  * @param t            double result
  * @param prec         zero precision
  * @return             true if convertion is successfull
 */
static bool toDouble(const string &str, double &t, const double prec)
{
    bool ret=false;
    string tmpStr = StringTools::lrTrim(str);
    if (tmpStr.find_first_not_of("0123456789.-+e") != string::npos)
        return false;
    stringstream sStr(tmpStr);
    if (!(sStr >> t).fail()) {
        ret=true;
        if ( fabs(t) < prec) {
            t=0;
        }
    }
    return ret;
}

/**
* @brief   If a string is enclosed in double quotes then the doubles quotes will be cleaned.
* @param str          String to be cleaned
*/
void StringTools::cleanFromDoubleQuotes(string &str)
{
    size_t firstQuote, lastQuote;
    if ((firstQuote = str.find_first_of("\"")) != string::npos)
        if ((lastQuote = str.find_last_of("\"")) != string::npos)
            str = str.substr(firstQuote + 1, lastQuote - firstQuote - 1);
}


/**
* @brief   Cleans a string from a specific string
* @param str          String to be cleaned
* @param str1         substring to be cleaned in str
*/
string StringTools::cleanFromString(const string &str, const string &str1)
{
    size_t pos;
    string tmpString = str;
    pos = tmpString.find(str1);

    while (pos != string::npos)
    {
        tmpString.erase(pos, str1.length());
        pos = tmpString.find(str1);
    }

    return tmpString;

}

/**
    * encodes xml expressions in a string
    * @param           string to be encoded
    * @return			encoded string
    */
string StringTools::encodeXml(string &str)
{
    string tmpString = str;
    tmpString = replaceString(tmpString, "&", "&amp;");
    tmpString = replaceString(tmpString, "\"", "&quot;");
    tmpString = replaceString(tmpString, "'", "&apos;");
    tmpString = replaceString(tmpString, ">", "&gt;");
    tmpString = replaceString(tmpString, "<", "&lt;");
    return tmpString;
}

/**
     * @brief  iterates over words in a string, on each call the next word is given back * @param str       string to iterate over
     * @param word      return of next word
     * @param pos       current position in string, start iteration with 0
     * @param quote     return if the word was quoted
     * @param qc        quote char / '\0' no quote char
     * @param sep       separating chars
     * @return          false if no more word found
     */
bool StringTools::iterWords(const string &str, string &word, size_t &pos, bool &quote, char qc, const char *sep)
{
    size_t start, end;
    bool dquote = false;

    // start of next word
    if (pos != string::npos)
        pos = str.find_first_not_of(sep, pos);

    if (pos == string::npos)
    {
        word.clear();
        return false;
    }

    quote = (qc != '\0' && qc == str[pos]);

    // end of next word
    if (!quote)
    {
        start = pos;
        end = str.find_first_of(sep, start);
    }
    else
    {
        start = pos + 1;
        end = 0;

        while (end == 0)
        {
            pos = str.find(qc, pos + 1);

            if (pos == string::npos)
            {
                // no closing quote
                end = string::npos;
            }
            else if (pos == str.size() - 1 || str[pos + 1] != qc)
            {
                // closing quote ending the word
                end = pos - 1;
            }
            else
            {
                // double quote is treated as one quote in the result word
                if (!dquote)
                    word = str.substr(start, pos + 1 - start);
                else
                    word += str.substr(start, pos + 1 - start);

                dquote = true;
                start = pos + 2;
                pos++;
            }
        }
    }

    // get next word
    if (!dquote)
        word = str.substr(start, (end == string::npos ? string::npos : (end - start)));
    else
        word += str.substr(start, (end == string::npos ? string::npos : (end - start)));

    // if not quoted search for a quote inside of the word
    if (!quote && qc != '\0')
    {
        pos = word.find(qc);
        if (pos != string::npos)
        {
            // word ends before the quote
            word = word.substr(0, pos);
            end = start + pos;
        }
    }

    pos = (end == string::npos ? string::npos : (quote ? end + 1 : end));

    return true;
}



/**
 * @brief cleans a string from all witeSpaces
 * @param str      string to be cleaned
 */
void StringTools::cleanFromWhiteSpaces(string &str)
{
    size_t pos;
    string tmpString = str;
    pos = tmpString.find_first_of(WHITE_SPACES);

    while (pos != string::npos)
    {
        tmpString.erase(pos, 1);
        pos = tmpString.find_first_of(WHITE_SPACES);
    }

    str = tmpString;

}

/**
* @brief returns a problem name w/o the extension .mps
* @param probName
* @return problem name
*/
string StringTools::problemName(const string &probName)
{
    size_t t = probName.find(".mps");
    if (t != string::npos)
        return probName.substr(0, t);
    else
        return probName;
}


/**
* @brief returns the path of a problem
*
* @param probName
* @return problem path
*/
string StringTools::problemPath(const string  &probName)
{
    size_t t;
    string str = replaceString(probName, "\\", "/");

    t = str.find_last_of("/");
    if (t != string::npos)
        return str.substr(0, t + 1);
    else
        return "";
}

/**
* @brief returns the name of a model
*
* @param probName
* @return model name w/o extension
*/
string StringTools::modelName(const string &probName)
{
    size_t t;
    string str = replaceString(probName, "\\", "/");
    string str1 = "";

    t = str.find_last_of("/");
    if (t != string::npos)
        str1 = str.substr(t + 1);
    else
        str1 = str;

    t = str1.find(".mps");
    if (t != string::npos)
        return str1.substr(0, t);
    else
    {
        return str1;
    }

}

/**
 * @brief Returns true if a string was found in a vector of strings
 * @param vec      vector of strings
 * @param str      string to be found in vec
 * @return         true if str was found in vec
 */
bool StringTools::findInStringVector(vector<string> &vec, const string &str) {
    return find(vec.begin(), vec.end(), str) != vec.end();
}


