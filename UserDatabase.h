#ifndef USERDATABASE_INCLUDED
#define USERDATABASE_INCLUDED

#include <string>
#include <vector>
#include "treemm.h"

class User;

class UserDatabase
{
  public:
    UserDatabase();
    ~UserDatabase(); // free up memory used by dynamically allocated users
    bool load(const std::string& filename); // load the database
    User* get_user_from_email(const std::string& email) const; // get a user from their email

  private:
    bool m_loaded;
    TreeMultimap<std::string, User*> m_emailMap;
    std::vector<User*> m_users;
};

#endif // USERDATABASE_INCLUDED
