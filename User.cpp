#include "User.h"

#include <string>
#include <vector>
using namespace std;

User::User(const string& full_name, const string& email,
           const vector<string>& watch_history) : m_name(full_name), m_email(email), m_watchHistory(watch_history)
{}

// get a user's name
string User::get_full_name() const
{
    return m_name;
}

// get a user's email
string User::get_email() const
{
    return m_email;
}

// get a user's watch history
vector<string> User::get_watch_history() const
{
    return m_watchHistory;
}
