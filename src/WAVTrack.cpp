#include "WAVTrack.h"
#include <iostream>

WAVTrack::WAVTrack(const std::string &title, const std::vector<std::string> &artists,
                   int duration, int bpm, int sample_rate, int bit_depth)
    : AudioTrack(title, artists, duration, bpm), sample_rate(sample_rate), bit_depth(bit_depth)
{

    std::cout << "WAVTrack created: " << sample_rate << "Hz/" << bit_depth << "bit" << std::endl;
}

void WAVTrack::load()
{
    std::cout << "[WAVTrack::load] Loading WAV: \"" << title << "\" at " << sample_rate << "Hz/" << bit_depth << "bit (uncompressed)..." << std::endl;

    long long size = duration_seconds * sample_rate * (bit_depth / 8) * 2;

    std::cout << "  → Estimated file size: " << size << " bytes" << std::endl
              << "  → Fast loading due to uncompressed format." << std::endl;
}

void WAVTrack::analyze_beatgrid()
{
    std::cout << "[WAVTrack::analyze_beatgrid] Analyzing beat grid for: \"" << title << "\"\n";
    int beats = (duration_seconds / 60.0) * bpm;

    std::cout << "  → Estimated beats: " << beats
              << "  → Precision factor: 1 (uncompressed audio)" << std::endl;
}

double WAVTrack::get_quality_score() const
{
    double base_score = 70;

    // sample rate additions
    if (sample_rate >= 44100)
    {
        base_score += 10;
    }
    if (sample_rate >= 96000)
    {
        base_score += 5;
    }
    // bit depth additions
    if (bit_depth >= 16)
    {
        base_score += 10;
    }
    if (bit_depth >= 24)
    {
        base_score += 5;
    }

    if (base_score > 100)
    {
        base_score = 100;
    }

    return base_score;
}

PointerWrapper<AudioTrack> WAVTrack::clone() const
{
    return PointerWrapper<AudioTrack>(new WAVTrack(*this));
}