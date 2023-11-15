/*
 * This file is part of the TrinityCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "TalentPackets.h"

ByteBuffer& operator>>(ByteBuffer& data, WorldPackets::Talent::PvPTalent& pvpTalent)
{
    data >> pvpTalent.PvPTalentID;
    data >> pvpTalent.Slot;
    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Talent::PvPTalent const& pvpTalent)
{
    data << uint16(pvpTalent.PvPTalentID);
    data << uint8(pvpTalent.Slot);
    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Talent::TalentInfo const& talentInfo)
{
    data << uint32(talentInfo.TalentID);
    data << uint8(talentInfo.Rank);
    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Talent::TalentGroupInfo const& talentGroupInfo)
{
    data << uint32(talentGroupInfo.GlyphInfo.size());
    data << uint32(talentGroupInfo.TalentInfos.size());
    data << uint32(talentGroupInfo.SpecID);

    for (uint16 glyph : talentGroupInfo.GlyphInfo)
        data << uint16(glyph);

    for (auto& talentInfo : talentGroupInfo.TalentInfos)
        data << talentInfo;

    return data;
}

WorldPacket const* WorldPackets::Talent::UpdateTalentData::Write()
{
    _worldPacket << uint8(ActiveGroup);
    _worldPacket << uint32(UnspentTalentPoints);
    _worldPacket << uint32(TalentGroupInfos.size());

    for (auto& talentGroupInfo : TalentGroupInfos)
        _worldPacket << talentGroupInfo;

    return &_worldPacket;
}

void WorldPackets::Talent::LearnTalents::Read()
{
    Talents.resize(_worldPacket.ReadBits(6));
    for (uint32 i = 0; i < Talents.size(); ++i)
        _worldPacket >> Talents[i];
}

WorldPacket const* WorldPackets::Talent::RespecWipeConfirm::Write()
{
    _worldPacket << int8(RespecType);
    _worldPacket << uint32(Cost);
    _worldPacket << RespecMaster;
    return &_worldPacket;
}

void WorldPackets::Talent::ConfirmRespecWipe::Read()
{
    _worldPacket >> RespecMaster;
    _worldPacket >> RespecType;
}

WorldPacket const* WorldPackets::Talent::LearnTalentFailed::Write()
{
    _worldPacket.WriteBits(Reason, 4);
    _worldPacket << int32(SpellID);
    _worldPacket << uint32(Talents.size());
    if (!Talents.empty())
        _worldPacket.append(Talents.data(), Talents.size());

    return &_worldPacket;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Talent::GlyphBinding const& glyphBinding)
{
    data << uint32(glyphBinding.SpellID);
    data << uint16(glyphBinding.GlyphID);
    return data;
}

WorldPacket const* WorldPackets::Talent::ActiveGlyphs::Write()
{
    _worldPacket << uint32(Glyphs.size());
    for (GlyphBinding const& glyph : Glyphs)
        _worldPacket << glyph;

    _worldPacket.WriteBit(IsFullUpdate);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

void WorldPackets::Talent::LearnPvpTalents::Read()
{
    Talents.resize(_worldPacket.read<uint32>());
    for (uint32 i = 0; i < Talents.size(); ++i)
        _worldPacket >> Talents[i];
}

WorldPacket const* WorldPackets::Talent::LearnPvpTalentFailed::Write()
{
    _worldPacket.WriteBits(Reason, 4);
    _worldPacket << int32(SpellID);
    _worldPacket << uint32(Talents.size());
    for (PvPTalent pvpTalent : Talents)
        _worldPacket << pvpTalent;

    return &_worldPacket;
}
