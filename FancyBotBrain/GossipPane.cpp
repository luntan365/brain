#include "GossipPane.h"

#include "Lua.h"
#include "WowOffsets.h"
#include <sstream>

void
GossipPane::Read()
{
    static constexpr uintptr_t optionsOffset = 
        StaticFields::STATIC_OPTIONS_OFFSET;
    static constexpr uintptr_t optionsEnd = 
        StaticFields::STATIC_OPTIONS_END;
    static constexpr uintptr_t optionsEndOffset = 
        StaticFields::STATIC_OPTIONS_END_OFFSET; 
    static constexpr uintptr_t optionsIter = 
        StaticFields::STATIC_OPTIONS_ITER;
    static constexpr uintptr_t gossipType = 
        StaticFields::STATIC_GOSSIP_TYPE;
    mOptions.clear();
    for (
        auto i = optionsOffset; 
        (i < optionsEnd && ReadOffset<int32_t>(i + optionsEndOffset) != -1);
        i += optionsIter)
    {
        GossipType type = (GossipType)ReadOffset<uint32_t>(i + gossipType);
        mOptions.push_back(type);
    }
    ReadOffsetInto(StaticFields::STATIC_GOSSIP_GUID, &mGossipNpcGuid);
}

bool
GossipPane::IsOpen() const
{
    return mGossipNpcGuid != 0;
}

std::vector<GossipOption> 
GossipPane::GetOptions() const
{
    return mOptions;
}

concurrency::task<void>
GossipPane::SelectOption(uint32_t index) const
{
    std::stringstream ss;
    ss << "SelectGossipOption(" << (index + 1) << ")";
    return AsyncExecuteLua(ss.str());
}

concurrency::task<void>
GossipPane::SelectFirstOptionWithType(GossipType type) const
{
    for (uint32_t i = 0; i < mOptions.size(); i++)
    {
        if (mOptions[i] == type)
        {
            return SelectOption(i);
        }
    }
    return concurrency::task_from_result();
}

