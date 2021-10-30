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

PrioTable::PrioTable(Tag tag, Array<String> const&tokens)
: tokens(tokens)
, data(AllocData(tokens.Count, [=](int, int){return tag; }))
{
    SetDumpString();
}

PrioTable::PrioTable(
    Array<String>const& tokens,
    Array<Array<Tag>> const&base,
    TagTable const& subTable,
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
        LeftTable,
        0);
}

PrioTable const PrioTable::Right(Array<String> const& tokens)const {
    return PrioTable(
        AllocTokens({ this->tokens, tokens }),
        data,
        RightTable,
        0);
}

PrioTable const PrioTable::CreateLeft(Array<String> const& tokens){
    return PrioTable(LowerTag, tokens);
}

PrioTable const PrioTable::CreateRight(Array<String> const& tokens){
    return PrioTable(HigherTag, tokens);
}

PrioTable const PrioTable::ParenthesisLevelLeft(Array<String> leftToken, Array<String> rightToken)const{
    return Level(ParenthesisTableLeft, leftToken, rightToken);
}

PrioTable const PrioTable::ParenthesisLevelRight(Array<String> leftToken, Array<String> rightToken)const{
    return Level(ParenthesisTableRight, leftToken, rightToken);
}

PrioTable const PrioTable::ThenElseLevel(Array<String> leftToken, Array<String> rightToken)const{
    return Level(ThenElseTable, leftToken, rightToken);
}

PrioTable const PrioTable::Level(TagTable const& subTable, Array<String> const&leftToken, Array<String>const&rightToken)const {
    return PrioTable(AllocTokens({ leftToken, tokens, rightToken }), data, subTable, static_cast<int>(leftToken.Count));
}

PrioTableConst::Tag const PrioTable::Relation(String const&newTokenName, String const&recentTokenName)const{
    return Relation(Index(newTokenName), Index(recentTokenName));
}

p_implementation(PrioTable, Array<String>, DumpData){
    if (Count == 0)
        return{nd(Count)};

    auto maxlen = tokens.Select<int>([](String const&t){return t.Count; })->Max().Value;
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


String const HWLib::Dump(Array<Array<Tag const>const> data){
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


String const HWLib::Dump(TagTable const&data){
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

    throw MissingFallbackEntryException();

}

PrioTableConst::Tag const PrioTable::Relation(int newIndex, int recentIndex)const
{
    a_is(tokens[newIndex], != , PrioTableConst::Start);
    a_is(tokens[recentIndex], != , PrioTableConst::End);
    return data[newIndex][recentIndex];
}

Array<String> const PrioTable::AllocTokens(Array<Array<String>> const &tokens)
{
    return tokens.ConvertMany<String>()->ToArray;
}

Array<Array<Tag>> const PrioTable::AllocData(size_t count, function<Tag(int, int)> getData)
{
    return Numbers(count)
        ->Select<Array<Tag>>
        ([=](int i)
            {
                return Numbers(count)
                    ->Select<Tag>
                    ([=](int j)
                        {
                            return getData(i, j);
                        }
                    )
                    ->ToArray;
            }
        )
        ->ToArray;
}

Tag const PrioTable::PrioChar(Array<Array<Tag>> const&base, TagTable const& subTable, int leftCount, int i, int j)
{
    d_here;
    fd(base, subTable, leftCount, i, j);

    auto baseCount = static_cast<int>(base.Count);
    auto iGroup = FindGroup(i, { leftCount, baseCount });
    auto jGroup = FindGroup(j, { leftCount, baseCount });

    if (iGroup == 1 && jGroup == 1)
        return_d(base[i - leftCount][j - leftCount]);

    if (iGroup == 2 && jGroup == 0)
    {
        auto result =
            i - leftCount - baseCount < j ?
                PrioTableConst::HigherTag :
                PrioTableConst::LowerTag;
        return_d(result);
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