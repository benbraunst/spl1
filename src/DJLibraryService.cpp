#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include <filesystem>

DJLibraryService::DJLibraryService(const Playlist &playlist)
    : playlist(playlist), library() {}
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
            std::cout << "MP3Track created: " << track.extra_param1 << "kbps";
            counter++;
        }
        else if (track.type == "WAV")
        {
            library.push_back(new WAVTrack(track.title, track.artists, track.duration_seconds,
                                           track.bpm, track.extra_param1, track.extra_param2));
            std::cout << "WAVTrack created: " << track.extra_param1 << "Hz/" << track.extra_param2 << "bit";
            counter++;
        }
    }

    std::cout << " [INFO] Track library built: " << counter << " tracks loaded";
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
    // Your implementation here
    return nullptr; // Placeholder
}

void DJLibraryService::loadPlaylistFromIndices(const std::string &playlist_name,
                                               const std::vector<int> &track_indices)
{
    // Your implementation here

    std::cout << "[INFO] Loading playlist: " << playlist_name;
    Playlist *playlist = new Playlist(playlist_name);

    int counter = 0;

    for (int index : track_indices)
    {
        if (index < library.size() && index >= 1)
        { // Validate index base (DEL)
            PointerWrapper<AudioTrack> track = library[index - 1]->clone();
            if (!track)
            {
                std::cerr << "[ERROR] error while cloning track";
            }
            else
            {
                AudioTrack *raw_track = track.release();
                raw_track->load();
                raw_track->analyze_beatgrid();

                playlist->add_track(raw_track);

                std::cout << "Added ’" << raw_track->get_title() << "’ to playlist ’" << playlist_name << "'";
                counter++;
            }
        }
        else
        {
            std::cout << "[WARNING] Invalid track index: " << index;
        }
    }
    std::cout << "[INFO] Playlist loaded:" << playlist_name << " (" << counter << " tracks)";

    // For now, add a placeholder to fix the linker error
    (void)playlist_name; // Suppress unused parameter warning
    (void)track_indices; // Suppress unused parameter warning
}
/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const
{
    // Your implementation here
    return std::vector<std::string>(); // Placeholder
}
