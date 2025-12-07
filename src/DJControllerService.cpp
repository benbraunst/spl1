#include "DJControllerService.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>

DJControllerService::DJControllerService(size_t cache_size)
    : cache(cache_size) {}

int DJControllerService::loadTrackToCache(AudioTrack &track)
{
    if (cache.contains(track.get_title())) // HIT case
    {
        cache.get(track.get_title());
        return 1;
    }

    else // MISS case
    {
        PointerWrapper<AudioTrack> clonedTrackPtr = track.clone();

        if (!clonedTrackPtr)
        {
            std::cerr << "[ERROR] Track: " << track.get_title() << " failed to clone for cache loading\n";
            return 0;
        }

        clonedTrackPtr->load();
        clonedTrackPtr->analyze_beatgrid();

        bool evicted = cache.put(std::move(clonedTrackPtr));
        return evicted ? -1 : 0;
    }
}

void DJControllerService::set_cache_size(size_t new_size)
{
    cache.set_capacity(new_size);
}
// implemented
void DJControllerService::displayCacheStatus() const
{
    std::cout << "\n=== Cache Status ===\n";
    cache.displayStatus();
    std::cout << "====================\n";
}

AudioTrack *DJControllerService::getTrackFromCache(const std::string &track_title)
{
    AudioTrack *trackPointer = cache.get(track_title);
    if (trackPointer == nullptr)
    {
        std::cerr << "[ERROR] Track: " << track_title << " not found in Cache\n";
    }
    return trackPointer;
}