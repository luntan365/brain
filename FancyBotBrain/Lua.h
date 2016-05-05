#pragma once

#include <vector>
#include <string>

void ExecuteLua(const std::string& script);

std::string GetLuaResult(
    const std::string& script,
    const std::string& argument
);

static const std::vector<std::string> LUA_FUNCTION_DEFS = {
    "function useItemInBag(totalSlots, bagnumber, item) for j = 1,totalSlots do "
        "link = GetContainerItemLink(bagnumber,j) "
        "if ( link  ) then link = gsub(link,'^.*%[(.*)%].*$','%1') "
        "if ( string.lower(item) == string.lower(link) ) then "
        "UseContainerItem(bagnumber,j) return 1 end end end return 0 end",

    "function IsVendorOpen() "
        "if MerchantFrame:IsVisible() "
        "then return 'true' "
        "else return 'false' end end",
};

void RegisterLuaFunctions();
