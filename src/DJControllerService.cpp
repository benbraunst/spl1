#include "DJControllerService.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>

DJControllerService::DJControllerService(size_t cache_size)
    : cache(cache_size) {}
/**
 * TODO: Implement loadTrackToCache method
 */
int DJControllerService::loadTrackToCache(AudioTrack &track)
{
    // Your implementation here

    // HIT case
    if (cache.contains(track.get_title()))
    {
        cache.get(track.get_title()); // validate (DEL)
        return 1;
    }

    // MISS case
    PointerWrapper<AudioTrack> clonedTrackPtr = track.clone(); // validate (DEL)

    if (!clonedTrackPtr)
    {
        std::cerr << "[ERROR] Track: " << track.get_title() << " failed to clone\n"; // which error to log (DEL)
        return 0;
    }

    clonedTrackPtr->load();
    clonedTrackPtr->analyze_beatgrid();

    bool evicted = cache.put(std::move(clonedTrackPtr));
    return evicted ? -1 : 0;
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

/**
 * TODO: Implement getTrackFromCache method
 */
AudioTrack *DJControllerService::getTrackFromCache(const std::string &track_title)
{
    AudioTrack *trackPointer = cache.get(track_title); // nothing related to PointerWrapper? (DEL)
    if (trackPointer == nullptr)
    {
        std::cerr << "[ERROR] Track: " << track_title << " failed to clone\n"; // which error to log (DEL)
    }
    return trackPointer;
}