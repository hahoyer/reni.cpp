#include "Import.h"
#include "PrioTable.h"
#include "PrioTableConst.h"
#include "../HWLib/String.h"

using namespace HWLang;
using namespace HWLib;


PrioTable::PrioTable()
{
    SetDumpString();
}


PrioTable::PrioTable(PrioTable const&other)
: tokens(other.tokens)
, data(other.data)
{
    SetDumpString();
};

PrioTable::PrioTable(PrioTableConst::Tag tag, Array<String> const&tokens)
: tokens(tokens)
, data(AllocData(tokens.Count, [=](int, int){return tag; }))
{
    SetDumpString();
}

PrioTable::PrioTable(
    Array<String>const& tokens,
    Array<Array<PrioTableConst::Tag>> const&base,
    PrioTableConst::TagTable const& subTable,
    int leftCount)
    : tokens(tokens)
    , data(AllocData(tokens.Count, [=,&base,&subTable](int i, int j){return PrioChar(base, subTable, leftCount, i, j); }))
{
    SetDumpString();
};

PrioTable const PrioTable::Left(Array<String> const& tokens)const {
    return PrioTable(
        AllocTokens({ this->tokens, tokens }),
        data,
        PrioTableConst::LeftTable,
        0);
}

PrioTable const PrioTable::Right(Array<String> const& tokens)const {
    return PrioTable(
        AllocTokens({ this->tokens, tokens }),
        data,
        PrioTableConst::RightTable,
        0);
}

PrioTable const PrioTable::CreateLeft(Array<String> const& tokens){
    return PrioTable(PrioTableConst::LowerTag, tokens);
}

PrioTable const PrioTable::CreateRight(Array<String> const& tokens){
    return PrioTable(PrioTableConst::HigherTag, tokens);
}

PrioTable const PrioTable::ParenthesisLevel(char const* leftToken, char const* rightToken)const {
    return Level(PrioTableConst::ParenthesisTable, { leftToken }, { rightToken });
}

PrioTable const PrioTable::ParenthesisLevel(Array<String> leftToken, Array<String> rightToken)const{
    return Level(PrioTableConst::ParenthesisTable, leftToken, rightToken);
}

PrioTable const PrioTable::ThenElseLevel(Array<String> leftToken, Array<String> rightToken)const{
    return Level(PrioTableConst::ThenElseTable, leftToken, rightToken);
}

PrioTable const PrioTable::Level(PrioTableConst::TagTable const& subTable, Array<String> const&leftToken, Array<String>const&rightToken)const {
    return PrioTable(AllocTokens({ leftToken, tokens, rightToken }), data, subTable, leftToken.Count);
}

PrioTableConst::Tag const PrioTable::Relation(String const&newTokenName, String const&recentTokenName)const{
    return Relation(Index(newTokenName), Index(recentTokenName));
}

p_implementation(PrioTable, Array<String>, DumpData){
    if (Count == 0)
        return{nd(Count)};

    auto maxlen = *tokens.Select<int>([](String const&t){return t.Count; })->Max();
    auto head0 = String().CastLeft(maxlen);
    head0 += "    ";
    auto head1 = head0;
    String result;
    for (auto i = 0; i < Count; i++){
        auto ii = HWLib::Dump(i + 10000);
        head0 += ii[3];
        head1 += ii[4];
        result += tokens[i].CastLeft(maxlen) + " " + ii.Part(3, 2) + " ";
        for (auto j = 0; j < Count; j++)
            result += String(data[i][j].value);
        result += "\n";
    }
    return{ "\n" + head0 + "\n" + head1 + "\n" + result };
}

bool PrioTable::Trace = false;


String const HWLib::Dump(Array<Array<HWLang::PrioTableConst::Tag const>const> data){
    String head0 = "\n .- ";
    String head1 = "\n |  ";
    String result;
    for (auto i = 0; i < data.Count; i++){
        auto ii = HWLib::Dump(i + 10000);
        head0 += ii[3];
        head1 += ii[4];
        result += String(" ") + ii.Part(3, 2) + " ";
        for (auto j = 0; j < data.Count; j++)
            result += String(data[i][j].value);
        result += "\n";
    }
    return (head0 + head1 + "\n" + result).Indent();
};


String const HWLib::Dump(HWLang::PrioTableConst::TagTable const&data){
    auto count = int(data.size());
    a_is(count, == , 3);

    char * * d = new char*[3];
    auto i = 0;
    for (auto l : data){
        d[i] = new char[3];
        auto j = 0;
        for (auto ll : l){
            d[i][j] = ll.value;
            j++;
        };
        i++;
    };

    String head0 = "\n .- ";
    String head1 = "\n |  ";
    String result;
    for (auto i = 0; i < count; i++){
        auto ii = HWLib::Dump(i + 10000);
        head0 += ii[3];
        head1 += ii[4];
        result += String(" ") + ii.Part(3, 2) + " ";
        for (auto j = 0; j < count; j++)
            result += String(d[i][j]);
        result += "\n";
    };

    return (head0 + head1 + "\n" + result).Indent();
};


int const PrioTable::Index(String const&name)const
{
    for (auto i = 0; i < Count; i++)
        if (tokens[i] == name)
            return (i);

    for (auto i = 0; i < Count; i++)
        if (tokens[i] == PrioTableConst::Any)
            return (i);

    throw String("missing ") + PrioTableConst::Any + " entry in priority table";

}

PrioTableConst::Tag const PrioTable::Relation(int newIndex, int recentIndex)const
{
    a_is(tokens[newIndex], != , PrioTableConst::Start);
    a_is(tokens[recentIndex], != , PrioTableConst::End);
    return data[newIndex][recentIndex];
}

Array<String> const PrioTable::AllocTokens(Array<Array<String>> const &tokens){
    return tokens.ConvertMany<String>()->ToArray;
}

Array<Array<PrioTableConst::Tag>> const PrioTable::AllocData(int count, function<PrioTableConst::Tag(int, int)> getData)
{
    return Array<Array<PrioTableConst::Tag>>(count, [=](int i)
    {
        return Array<PrioTableConst::Tag>(count, [=](int j)
        {
            return getData(i, j);
        });
    });
}

PrioTableConst::Tag const PrioTable::PrioChar(Array<Array<PrioTableConst::Tag>> const&base, PrioTableConst::TagTable const& subTable, int leftCount, int i, int j)
{
    d_here;
    fd(base, subTable, leftCount, i, j);

    auto baseCount = base.Count;
    auto iGroup = FindGroup(i, { leftCount, baseCount });
    auto jGroup = FindGroup(j, { leftCount, baseCount });

    if (iGroup == 1 && jGroup == 1)
        return_d(base[i - leftCount][j - leftCount]);

    if (iGroup == 2 && jGroup == 0)
    {
        switch (sign(leftCount + baseCount - i + j))
        {
        case -1:
            return_d(PrioTableConst::LowerTag);
        case 0:
            return_d(PrioTableConst::MatchTag);
        case 1:
            return_d(PrioTableConst::HigherTag);
        default:
            throw "Unexpected sign";
        }
    }
    return_d((subTable.begin()[iGroup]).begin()[jGroup]);
};

int PrioTable::FindGroup(int i, Array<int>const&counts)
{
    auto result = 0;
    for (auto count : counts)
    {
        i -= count;
        if (i < 0)
            return result;
        result++;
    };
    return result;
}