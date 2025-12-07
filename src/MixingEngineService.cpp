#include "MixingEngineService.h"
#include <iostream>
#include <memory>
#include <cmath>

MixingEngineService::MixingEngineService()
    : decks(),
      active_deck(1),
      auto_sync(false),
      bpm_tolerance(0)
{
    std::cout << "[MixingEngineService] Initialized with 2 empty decks.\n";
}

MixingEngineService::~MixingEngineService()
{
    std::cout << "[MixingEngineService] Cleaning up decks..." << std::endl;
    delete decks[0];
    delete decks[1];

    decks[0] = nullptr;
    decks[1] = nullptr;
}

/**
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack &track)
{
    // (a)
    std::cout << "\n=== Loading Track to Deck ===\n";

    // (b)
    PointerWrapper<AudioTrack> clonedTrackPtr = track.clone();

    // (c)
    if (!clonedTrackPtr)
    {
        std::cerr << "[ERROR] Track: \""
                  << track.get_title()
                  << "\" failed to clone\n";
        return -1;
    }

    size_t target_deck;
    target_deck = 1 - active_deck;
    std::cout << "[Deck Switch] Target deck: " << target_deck << std::endl;

    clonedTrackPtr->load();
    clonedTrackPtr->analyze_beatgrid();

    if (decks[target_deck])
    {
        delete decks[target_deck];
        decks[target_deck] = nullptr;
    }

    // BPM Management:
    if (decks[active_deck])
    {
        if (!can_mix_tracks(clonedTrackPtr) && auto_sync)
        {
            sync_bpm(clonedTrackPtr);
        }
    }
    else
    {
        std::cout << "[Sync BPM] Cannot sync - one of the decks is empty.\n";
    }

    decks[target_deck] = clonedTrackPtr.release();

    std::cout << "[Load Complete] '" << decks[target_deck]->get_title()
              << "' is now loaded on deck " << target_deck << std::endl;

    // (j) - Removes after new instructions and output
    // if (decks[active_deck] != nullptr && active_deck != target_deck)
    // {
    //     std::cout << "[Unload] Unloading previous deck " << active_deck
    //               << " (" << decks[active_deck]->get_title() << ")\n";
    //     delete decks[active_deck];
    //     decks[active_deck] = nullptr;
    // }

    // (k)
    active_deck = target_deck;

    std::cout << "[Active Deck] Switched to deck " << target_deck << "\n";

    return static_cast<int>(active_deck);
}

/**
 * @brief Display current deck status
 */
void MixingEngineService::displayDeckStatus() const
{
    std::cout << "\n=== Deck Status ===\n";
    for (size_t i = 0; i < 2; ++i)
    {
        if (decks[i])
            std::cout << "Deck " << i << ": " << decks[i]->get_title() << "\n";
        else
            std::cout << "Deck " << i << ": [EMPTY]\n";
    }
    std::cout << "Active Deck: " << active_deck << "\n";
    std::cout << "===================\n";
}

/**
 *
 * @brief Check if two tracks can be mixed based on BPM difference.
 *
 * @param track: Track to check for mixing compatibility
 * @return: true if BPM difference <= tolerance, false otherwise
 */
bool MixingEngineService::can_mix_tracks(const PointerWrapper<AudioTrack> &track) const
{
    if (!track)
    {
        return false;
    }

    if (!decks[active_deck])
    {
        return false;
    }

    int active_deck_bpm = decks[active_deck]->get_bpm();
    int track_bpm = track->get_bpm();
    int bpm_difference = std::abs(active_deck_bpm - track_bpm);
    return bpm_difference <= bpm_tolerance;
}

/**
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack> &track) const
{
    if (track && decks[active_deck])
    {
        int track_bpm = track->get_bpm();
        int active_deck_bpm = decks[active_deck]->get_bpm();

        int avarage_bpm = (track_bpm + active_deck_bpm) / 2;

        std::cout << "[Sync BPM] Syncing BPM from "
                  << track_bpm << " to " << avarage_bpm << "\n";

        track->set_bpm(avarage_bpm);
    }
}