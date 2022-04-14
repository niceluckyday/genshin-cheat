#include "pch-il2cpp.h"
#include "Chest.h"

#include "util.h"

namespace cheat::game
{

	Chest::Chest(app::BaseEntity* rawEntity) : Entity(rawEntity)
	{
	}

	cheat::game::Chest::ItemType Chest::itemType()
	{
		if (!isLoaded())
			return ItemType::None;

		if (m_ItemType)
			return *m_ItemType;

		if (name().find("TreasureBox") != std::string::npos)
			m_ItemType = ItemType::Chest;
		else if (name().find("Search") != std::string::npos || name().find("JunkChest") != std::string::npos)
			m_ItemType = ItemType::Investigate;
		else if (name().find("BookPage") != std::string::npos)
			m_ItemType = ItemType::BookPage;
		else if (name().find("BGM") != std::string::npos)
			m_ItemType = ItemType::BGM;
		else
			m_ItemType = ItemType::None;
		
		return *m_ItemType;
	}

	cheat::game::Chest::ChestState Chest::chestState()
	{
		if (!isLoaded())
			return ChestState::None;

		if (m_ChestState)
			return *m_ChestState;

		auto chestPlugin = game::GetLCPlugin<app::LCChestPlugin>(raw(), *app::LCChestPlugin__TypeInfo);
		if (chestPlugin == nullptr ||
			chestPlugin->fields._owner == nullptr ||
			chestPlugin->fields._owner->fields._dataItem == nullptr)
		{
			m_ChestState = ChestState::Invalid;
			return *m_ChestState;
		}
		
		auto state = static_cast<app::GadgetState__Enum>(chestPlugin->fields._owner->fields._dataItem->fields.gadgetState);
		switch (state)
		{
		case app::GadgetState__Enum::ChestLocked:
			m_ChestState = ChestState::Locked;
			break;
		case app::GadgetState__Enum::ChestRock:
			m_ChestState = ChestState::InRock;
			break;
		case app::GadgetState__Enum::ChestFrozen:
			m_ChestState = ChestState::Frozen;
			break;
		case app::GadgetState__Enum::ChestBramble:
			m_ChestState = ChestState::Bramble;
			break;
		case app::GadgetState__Enum::ChestTrap:
			m_ChestState = ChestState::Trap;
			break;
		case app::GadgetState__Enum::ChestOpened:
			m_ChestState = ChestState::Invalid;
			break;
		default:
			m_ChestState = ChestState::None;
			break;
		}

		return *m_ChestState;
	}

	cheat::game::Chest::ChestRarity Chest::chestRarity()
	{
		if (!isLoaded())
			return ChestRarity::Unknown;

		if (m_ChestRarity)
			return *m_ChestRarity;

		if (itemType() != ItemType::Chest)
		{
			m_ChestRarity = ChestRarity::Unknown;
			return ChestRarity::Unknown;
		}

		auto rarityIdPos = name().find('0');
		if (rarityIdPos == std::string::npos)
		{
			m_ChestRarity = ChestRarity::Unknown;
			return ChestRarity::Unknown;
		}
		
		int rarityId = name()[rarityIdPos + 1] - 48;
		switch (rarityId)
		{
		case 1:
			m_ChestRarity = ChestRarity::Common;
			break;
		case 2:
			m_ChestRarity = ChestRarity::Exquisite;
			break;
		case 4:
			m_ChestRarity = ChestRarity::Precious;
			break;
		case 5:
			m_ChestRarity = ChestRarity::Luxurious;
			break;
		case 6:
			m_ChestRarity = ChestRarity::Remarkable;
			break;
		default:
			m_ChestRarity = ChestRarity::Unknown;
			break;
		}

		return *m_ChestRarity;
	}

	ImColor Chest::chestColor()
	{
		switch (itemType())
		{
		case ItemType::Chest:
		{
			switch (chestRarity())
			{
			case ChestRarity::Common:
				return ImColor(255, 255, 255);
			case ChestRarity::Exquisite:
				return ImColor(0, 218, 255);
			case ChestRarity::Precious:
				return ImColor(231, 112, 255);
			case ChestRarity::Luxurious:
				return ImColor(246, 255, 0);
			case ChestRarity::Remarkable:
				return ImColor(255, 137, 0);
			case ChestRarity::Unknown:
			default:
				return ImColor(72, 72, 72);
			}
		}
		case ItemType::Investigate:
		case ItemType::BookPage:
		case ItemType::BGM:
			return ImColor(104, 146, 163);
		case ItemType::None:
		default:
			return ImColor(72, 72, 72);
		}
	}

	std::string Chest::minName()
	{
		switch (itemType())
		{
		case ItemType::Chest:
		{
			switch (chestRarity())
			{
			case ChestRarity::Common:
				return "CR1";
			case ChestRarity::Exquisite:
				return "CR2";
			case ChestRarity::Precious:
				return "CR3";
			case ChestRarity::Luxurious:
				return "CR4";
			case ChestRarity::Remarkable:
				return "CR5";
			case ChestRarity::Unknown:
			default:
				return "UNK";
			}
		}
		case ItemType::Investigate:
			return "INV";
		case ItemType::BookPage:
			return "BPG";
		case ItemType::BGM:
			return "BGM";
		case ItemType::None:
		default:
			return "UNK";
		}
	}

}