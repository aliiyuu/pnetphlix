#ifndef MOVIEDATABASE_INCLUDED
#define MOVIEDATABASE_INCLUDED

#include <string>
#include <vector>
#include "treemm.h"

class Movie;

class MovieDatabase
{
  public:
    MovieDatabase();
    ~MovieDatabase();
    bool load(const std::string& filename);
    Movie* get_movie_from_id(const std::string& id) const;
    std::vector<Movie*> get_movies_with_director(const std::string& director) const;
    std::vector<Movie*> get_movies_with_actor(const std::string& actor) const;
    std::vector<Movie*> get_movies_with_genre(const std::string& genre) const;

  private:
    bool m_loaded;
    std::vector<Movie*> m_movies;
    std::vector<std::string> stringToVector(const std::string& str) const // convert a comma-separated list into a vector of attributes
    {
        std::string current;
        std::vector<std::string> v;
        
        for (int i = 0; i < str.size(); i++)
        {
            if (str.at(i) == ',')
            {
                v.push_back(current);
                current = "";
            }
            else current += str.at(i);
        }
        v.push_back(current);
        
        return v;
    }
    std::string makeLower(const std::string& str) const // returns the lowercase version of a string
    {
        std::string temp;
        for (int i = 0; i < str.size(); i++)
            temp += tolower(str.at(i));
        return temp;
    }
    TreeMultimap<std::string, Movie*> m_idMap; // associates movie ids to movies
    TreeMultimap<std::string, Movie*> m_directorMap; // associates director names to movies
    TreeMultimap<std::string, Movie*> m_actorMap; // associates actor names to movies
    TreeMultimap<std::string, Movie*> m_genreMap; // associates genres to movies
};

#endif // MOVIEDATABASE_INCLUDED
