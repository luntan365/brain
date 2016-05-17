#pragma once

#include <cstdint>
#include <ppltasks.h>
#include <vector>

enum GossipType : uint32_t
{
    GOSSIP = 0,
    VENDOR = 1,
    TAXI = 2,
    TRAINER = 3,
    HEALER = 4,
    BINDER = 5,
    BANKER = 6,
    PETITION = 7,
    TABARD = 8,
    BATTLEMASTER = 9,
    AUCTIONEER = 10,
};


typedef GossipType GossipOption;

class GossipPane
{
public:
    void Read();

    bool IsOpen() const;

    std::vector<GossipOption> GetOptions() const;

    concurrency::task<void> SelectOption(uint32_t index) const;

    concurrency::task<void> SelectFirstOptionWithType(GossipType type) const;

private:
    uint64_t mGossipNpcGuid;
    std::vector<GossipOption> mOptions;
};
