#include "MovieDatabase.h"
#include "treemm.h"
#include "Movie.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

MovieDatabase::MovieDatabase() : m_loaded(false)
{}

MovieDatabase::~MovieDatabase()
{
    for (int i = 0; i < m_movies.size(); i++)
        delete m_movies[i];
}

bool MovieDatabase::load(const string& filename)
{
    if (m_loaded) return false; // only load a database once
    ifstream movieFile(filename);
    if (!movieFile)  return false; // unable to load movies
    m_loaded = true;
    string id, title, year, directors, actors, genres;
    float rating = 0;
    int lineCount = 0;
    
    string s;
    
    // process each line of a movie record
    while (getline(movieFile, s))
    {
        switch (lineCount)
        {
            case 0: // movie id
                id = s;
                lineCount++;
                break;
            case 1: // movie title
                title = s;
                lineCount++;
                break;
            case 2: // release year
                year = s;
                lineCount++;
                break;
            case 3: // list of directors
                directors = s;
                lineCount++;
                break;
            case 4: // list of actors
                actors = s;
                lineCount++;
                break;
            case 5: // list of genres
                genres = s;
                lineCount++;
                break;
            case 6:
            {
                rating = stof(s);
                lineCount++;
                Movie* m = new Movie(id, title, year, stringToVector(makeLower(directors)), stringToVector(makeLower(actors)), stringToVector(makeLower(genres)), rating); // create a new movie once all lines have been processed
                
                if (m)
                {
                    m_movies.push_back(m);
                    m_idMap.insert(makeLower(id), m); 
                    
                    for (int i = 0; i < m->get_directors().size(); i++)
                        m_directorMap.insert(m->get_directors()[i], m); 
                    
                    for (int i = 0; i < m->get_actors().size(); i++)
                        m_actorMap.insert(m->get_actors()[i], m); 
                    
                    for (int i = 0; i < m->get_genres().size(); i++)
                        m_genreMap.insert(m->get_genres()[i], m); 
                }
            }
                break;
            default:
                lineCount = 0; // reset line counter when at an empty line
                break;
        }
    }
    
    return true; // finished loading
}

Movie* MovieDatabase::get_movie_from_id(const string& id) const
{
    TreeMultimap<string, Movie*>::Iterator it = m_idMap.find(makeLower(id)); // do a case-insensitive search of movie ids
    if (!it.is_valid()) return nullptr;
    else return it.get_value();
}

vector<Movie*> MovieDatabase::get_movies_with_director(const string& director) const
{
    vector <Movie*> movies; 
    TreeMultimap<string, Movie*>::Iterator it = m_directorMap.find(makeLower(director)); // do a case-insensitive search of movies with a certain director
    while (it.is_valid())
    {
        movies.push_back(it.get_value());
        it.advance();
    }
    return movies;
}

vector<Movie*> MovieDatabase::get_movies_with_actor(const string& actor) const
{
    vector <Movie*> movies;
    TreeMultimap<string, Movie*>::Iterator it = m_actorMap.find(makeLower(actor)); // do a case-insensitive search of movies with a certain actor
    while (it.is_valid())
    {
        movies.push_back(it.get_value());
        it.advance();
    }
    return movies;
}

vector<Movie*> MovieDatabase::get_movies_with_genre(const string& genre) const
{
    vector <Movie*> movies;
    TreeMultimap<string, Movie*>::Iterator it = m_genreMap.find(makeLower(genre)); // do a case-insensitive search of movies with a certain genre
    while (it.is_valid())
    {
        movies.push_back(it.get_value());
        it.advance();
    }
    return movies;
}
