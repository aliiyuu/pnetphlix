#include "Recommender.h"
#include "UserDatabase.h"
#include "MovieDatabase.h"
#include "User.h"
#include "Movie.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

Recommender::Recommender(const UserDatabase& user_database,
                         const MovieDatabase& movie_database) : m_userDB(user_database), m_movieDB(movie_database) {}

vector<MovieAndRank> Recommender::recommend_movies(const string& user_email, int movie_count) const
{
    vector<MovieCandidate> movieList; // list of possible recommendations (unfiltered)
    vector<MovieAndRank> recommendations; // final list of recommendations
    
    User* u = m_userDB.get_user_from_email(user_email);
    
    if (u)
    {
        vector<string> history = u->get_watch_history(); 
        sort(history.begin(), history.end()); // sort history 
        CompatibilityRecord cr = CompatibilityRecord(m_movieDB, history); 
        
        
        // for each actor present in the history, add the total score from the history to any movies that have that actor
        for (std::unordered_map<std::string, int>::iterator it = cr.getActors().begin(); it != cr.getActors().end(); it++)
        {
            vector<Movie*> movies = m_movieDB.get_movies_with_actor(it->first);
            for (int i = 0; i < movies.size(); i++)
                cr.addMovie(movieList, movies[i]->get_id(), movies[i]->get_title(), it->second, movies[i]->get_rating());
        }
        
        // for each director present in the history, add the total score from the history to any movies that have that director
        for (std::unordered_map<std::string, int>::iterator it = cr.getDirectors().begin(); it != cr.getDirectors().end(); it++)
        {
            vector<Movie*> movies = m_movieDB.get_movies_with_director(it->first);
            for (int i = 0; i < movies.size(); i++)
                cr.addMovie(movieList, movies[i]->get_id(), movies[i]->get_title(), it->second, movies[i]->get_rating());
        }
        
        // if there are already a lot of recommendations (significantly more than movie_count), then just look for movies that share genres with the movies in the history
        if (movieList.size() > history.size() + movie_count)
        {
            for (int i = 0; i < movieList.size(); i++)
            {
                Movie* m = m_movieDB.get_movie_from_id(movieList[i].m_id);
                for (int j = 0; j < m->get_genres().size(); j++)
                {
                    for (std::unordered_map<std::string, int>::iterator it = cr.getGenres().begin(); it != cr.getGenres().end(); it++)
                    {
                        if (m->get_genres()[j] == it->first)
                        {
                            movieList[i].m_score += it->second;
                            break;
                        }
                    }
                }
            }
        }
        else // for each genre present in the history, add the total score from the history to any movies that have that genre
        {
            for (std::unordered_map<std::string, int>::iterator it = cr.getGenres().begin(); it != cr.getGenres().end(); it++)
            {
                vector<Movie*> movies = m_movieDB.get_movies_with_genre(it->first);
                for (int i = 0; i < movies.size(); i++)
                    cr.addMovie(movieList, movies[i]->get_id(), movies[i]->get_title(), it->second, movies[i]->get_rating());
            }
        }
        
        vector<MovieCandidate> filteredList; // don't recommend movies in the history
        
        for (int i = 0; i < movieList.size(); i++)
        {
            if (!(binary_search(history.begin(), history.end(), movieList[i].m_id)))
                filteredList.push_back(movieList[i]);
        }
        
        sort(filteredList.begin(), filteredList.end()); // sort the list based on compatibility score, rating, and title
        
        int limit = (filteredList.size() < movie_count) ? filteredList.size() : movie_count;
        
        for (int i = 0; i < limit; i++) // add to final recommendations list
            recommendations.push_back(MovieAndRank(filteredList[i].m_id, filteredList[i].m_score));
    }
    
    return recommendations; // return all recommendations, or empty vector if nothing was found
}

Recommender::CompatibilityRecord::CompatibilityRecord(const MovieDatabase& mdb, std::vector<std::string> history)
{
    for (int i = 0; i < history.size(); i++)
    {
        Movie* m = mdb.get_movie_from_id(history[i]); 
      
        for (int i = 0; i < m->get_actors().size(); i++)
            addEntry(actors, m->get_actors()[i], 30); // for each actor that movie has, add 30 to the total score contribution from the history
        
        for (int i = 0; i < m->get_directors().size(); i++)
            addEntry(directors, m->get_directors()[i], 20); // for each director that movie has, add 20 to the total score contribution from the history
        
        for (int i = 0; i < m->get_genres().size(); i++)
            addEntry(genres, m->get_genres()[i], 1); // for each actor that movie has, add 1 to the total score contribution from the history
    }
}
