#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include <filesystem>
#include <vector>

DJLibraryService::DJLibraryService(const Playlist &playlist)
    : playlist(playlist.get_name()), library() {}
/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo> &library_tracks)
{
    // Todo: Implement buildLibrary method
    int counter = 0;
    for (SessionConfig::TrackInfo track : library_tracks)
    {
        if (track.type == "MP3")
        {
            library.push_back(new MP3Track(track.title, track.artists, track.duration_seconds,
                                           track.bpm, track.extra_param1, track.extra_param2));
            counter++;
        }
        else if (track.type == "WAV")
        {
            library.push_back(new WAVTrack(track.title, track.artists, track.duration_seconds,
                                           track.bpm, track.extra_param1, track.extra_param2));
            counter++;
        }
    }

    std::cout << "[INFO] Track library built: " << counter << " tracks loaded\n";
}

/**
 * @brief Display the current state of the DJ library playlist
 *
 */
void DJLibraryService::displayLibrary() const
{
    std::cout << "=== DJ Library Playlist: "
              << playlist.get_name() << " ===" << std::endl;

    if (playlist.is_empty())
    {
        std::cout << "[INFO] Playlist is empty.\n";
        return;
    }

    // Let Playlist handle printing all track info
    playlist.display();

    std::cout << "Total duration: " << playlist.get_total_duration() << " seconds" << std::endl;
}

/**
 * @brief Get a reference to the current playlist
 *
 * @return Playlist&
 */
Playlist &DJLibraryService::getPlaylist()
{
    // Your implementation here
    return playlist;
}

/**
 * TODO: Implement findTrack method
 *
 * HINT: Leverage Playlist's find_track method
 */
AudioTrack *DJLibraryService::findTrack(const std::string &track_title)
{
    return playlist.find_track(track_title);
}

void DJLibraryService::loadPlaylistFromIndices(const std::string &playlist_name,
                                               const std::vector<int> &track_indices)
{

    std::cout << "[INFO] Loading playlist: " << playlist_name << "\n";
    playlist = Playlist(playlist_name);
    int counter = 0;

    for (int index : track_indices)
    {
        if (static_cast<size_t>(index) > library.size() || index < 1)
        {
            std::cout << "[WARNING] Invalid track index: " << index << "\n";
        }
        else{
            PointerWrapper<AudioTrack> track = library[index - 1]->clone();
            if (!track)
            {
                std::cerr << "[ERROR] Track failed to clone\n";
            }
            else
            {
                track->load();
                track->analyze_beatgrid();
                AudioTrack *raw_track = track.release();

                playlist.add_track(raw_track);
                counter++;
            }
        }
    }
    std::cout << "[INFO] Playlist loaded: " << playlist_name << " (" << counter << " tracks)\n";
}

std::vector<std::string> DJLibraryService::getTrackTitles() const
{
    std::vector<AudioTrack*> tracks = playlist.getTracks();
    std::vector<std::string> titles;
    titles.reserve(tracks.size());

    for (AudioTrack* track: tracks){
        titles.insert(titles.begin(), track->get_title());
    }

    return titles; 
}
