#ifndef RECOMMENDER_INCLUDED
#define RECOMMENDER_INCLUDED

#include <string>
#include <vector>
#include <unordered_map>

class UserDatabase;
class MovieDatabase;

struct MovieAndRank
{
    MovieAndRank(const std::string& id, int score)
     : movie_id(id), compatibility_score(score)
    {}

    std::string movie_id;
    int compatibility_score;
};

class Recommender
{
  public:
    Recommender(const UserDatabase& user_database,
                const MovieDatabase& movie_database);
    std::vector<MovieAndRank> recommend_movies(const std::string& user_email,
                                               int movie_count) const;

  private:
    const UserDatabase& m_userDB;
    const MovieDatabase& m_movieDB;
    
    struct MovieCandidate // to be able to sort MovieAndRanks
    {
        MovieCandidate(const std::string& id, const std::string& name, int score, float rating)
            : m_id(id), m_name(name), m_score(score), m_rating(rating)
        {}
        
        bool operator<(const MovieCandidate& other) const 
        {
            if (m_score == other.m_score)
            {
                if (m_rating == other.m_rating) return m_name.compare(other.m_name) < 0;
                else return m_rating > other.m_rating;
            }
            else return m_score > other.m_score;
        }
        
        std::string m_id;
        std::string m_name;
        int m_score;
        float m_rating;
    };
    
    class CompatibilityRecord
    {
        public:
            CompatibilityRecord(const MovieDatabase& m, std::vector<std::string> history);
            std::unordered_map<std::string, int>& getActors() { return actors; }
            std::unordered_map<std::string, int>& getDirectors() { return directors; }
            std::unordered_map<std::string, int>& getGenres() { return genres; }
            void addMovie(std::vector<MovieCandidate>& contents, const std::string& id, const std::string& name, int score, float rating)
            {
                for (int i = 0; i < contents.size(); i++)
                {
                    if (contents[i].m_id == id) // if that movie already exists in the list of recommendations, increase its score by a certain amount
                    {
                        contents[i].m_score += score;
                        return;
                    }
                }
                contents.push_back(MovieCandidate(id, name, score, rating)); // if the movie is not in the list of recommendations, add it with an compatibility score
            }
        private:
            std::unordered_map<std::string, int> directors; // associate directors with total score from history
            std::unordered_map<std::string, int> actors; // associate actors with total score from history
            std::unordered_map<std::string, int> genres; // associate genres with total score from history
            // add an entry to the appropriate map to record the total score
            void addEntry(std::unordered_map<std::string, int>& contents, const std::string& name, int score)
            {
                for (std::unordered_map<std::string, int>::iterator it = contents.begin(); it != contents.end(); it++)
                {
                    if (it->first == name)
                    {
                        it->second += score;
                        return;
                    }
                }
                contents[name] = score;
            }
    };
};

#endif // RECOMMENDER_INCLUDED
