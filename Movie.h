#ifndef MOVIE_INCLUDED
#define MOVIE_INCLUDED

#include <string>
#include <vector>

class Movie
{
  public:
    Movie(const std::string& id, const std::string& title,
          const std::string& release_year,
          const std::vector<std::string>& directors,
          const std::vector<std::string>& actors,
          const std::vector<std::string>& genres, float rating);
    std::string get_id() const; // get movie id
    std::string get_title() const; // get movie title
    std::string get_release_year() const; // get release year
    float get_rating() const; // get rating
    std::vector<std::string> get_directors() const; // get list of directors
    std::vector<std::string> get_actors() const; // get list of actors
    std::vector<std::string> get_genres() const; // get list of genres

  private:
    std::string m_id;
    std::string m_title;
    std::string m_releaseYear;
    float m_rating;
    std::vector<std::string> m_directors;
    std::vector<std::string> m_actors;
    std::vector<std::string> m_genres;
};

#endif // MOVIE_INCLUDED
