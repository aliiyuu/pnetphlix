#include "UserDatabase.h"
#include "User.h"
#include <fstream>
#include <string>
#include <vector>
using namespace std;

UserDatabase::UserDatabase() : m_loaded(false)
{}

// free up memory used by dynamically allocated users
UserDatabase::~UserDatabase()
{
    for (int i = 0; i < m_users.size(); i++)
        delete m_users[i];
}

bool UserDatabase::load(const string& filename)
{
    if (m_loaded) return false; // only load once
    ifstream userFile(filename);
    if (!userFile)  return false; // unable to load the database
    m_loaded = true;
    string name, email;
    vector<string> watchHistory;
    int lineCount = 0;
    int movieCount = 0;
    
    string s;
    
    // process each line of a user record
    while (getline(userFile, s))
    {
        switch (lineCount)
        {
            case 0:
                name = s; // name
                lineCount++;
                break;
            case 1:
                email = s; // email
                lineCount++;
                break;
            case 2:
                movieCount = stoi(s); // count of movies watched
                lineCount++;
                break;
            case 3:
                watchHistory.push_back(s); // add a movie to the history
                movieCount--;
                if (movieCount == 0) // done processing movies
                {
                    User* u = new User(name, email, watchHistory); // create new users
                    if (u)
                    {
                        m_users.push_back(u);
                        m_emailMap.insert(u->get_email(), u);
                    }
                    watchHistory.clear(); // clear history in order to start on a different user
                    lineCount++; // update line count
                }
                break;
            default:
                lineCount = 0; // reset line count for a new record
                break;
        }
    }
    return true;
}

User* UserDatabase::get_user_from_email(const string& email) const
{
    TreeMultimap<string, User*>::Iterator it = m_emailMap.find(email); // find a user from the map
    if (!it.is_valid()) return nullptr;
    else return it.get_value();
}
