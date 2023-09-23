#ifndef USER_INCLUDED
#define USER_INCLUDED

#include <string>
#include <vector>

class User
{
  public:
    User(const std::string& full_name, const std::string& email,
         const std::vector<std::string>& watch_history);
    std::string get_full_name() const; // get a user's name
    std::string get_email() const; // get a user's email
    std::vector<std::string> get_watch_history() const; // get a user's watch history

  private:
    std::string m_name;
    std::string m_email;
    std::vector<std::string> m_watchHistory;
};

#endif // USER_INCLUDED
