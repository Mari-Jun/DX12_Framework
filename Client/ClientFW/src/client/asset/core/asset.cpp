#include "stdafx.h"
#include "client/asset/core/asset.h"

namespace client_fw
{
	bool operator<(const AssetInfo& asset1, const AssetInfo& asset2)
	{
		if (asset1.name != asset2.name)
			return asset1.name < asset2.name;
		else
			return asset1.path < asset2.path;
	}
}
