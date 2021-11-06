#include "Import.h"
#include "PrioTable.h"
#include "PrioTableConst.h"
#include "../HWLib/string.h"

using namespace HWLang;
using namespace HWLib;


PrioTable::PrioTable()
{
  SetDumpString();
}


PrioTable::PrioTable(const PrioTable& other)
  : tokens(other.tokens)
    , data(other.data)
{
  SetDumpString();
};

PrioTable::PrioTable(Tag tag, const Array<string>& tokens)
  : tokens(tokens)
    , data(AllocData(tokens.Count, [=](size_t, size_t) { return tag; }))
{
  SetDumpString();
}

PrioTable::PrioTable(
  const Array<string>& tokens,
  const Array<Array<Tag>>& base,
  const TagTable& subTable,
  size_t leftCount)
  : tokens(tokens)
    , data(AllocData(tokens.Count, [=,&base,&subTable](size_t i, size_t j) { return PrioChar(base, subTable, leftCount, i, j); }))
{
  SetDumpString();
};

PrioTable PrioTable::Left(const Array<string>& tokens) const
{
  return PrioTable(
    AllocTokens({this->tokens, tokens}),
    data,
    LeftTable,
    0);
}

PrioTable PrioTable::Right(const Array<string>& tokens) const
{
  return PrioTable(
    AllocTokens({this->tokens, tokens}),
    data,
    RightTable,
    0);
}

PrioTable PrioTable::CreateLeft(const Array<string>& tokens)
{
  return PrioTable(LowerTag, tokens);
}

PrioTable PrioTable::CreateRight(const Array<string>& tokens)
{
  return PrioTable(HigherTag, tokens);
}

PrioTable PrioTable::ParenthesisLevelLeft(Array<string> leftToken, Array<string> rightToken) const
{
  return Level(ParenthesisTableLeft, leftToken, rightToken);
}

PrioTable PrioTable::ParenthesisLevelRight(Array<string> leftToken, Array<string> rightToken) const
{
  return Level(ParenthesisTableRight, leftToken, rightToken);
}

PrioTable PrioTable::ThenElseLevel(Array<string> leftToken, Array<string> rightToken) const
{
  return Level(ThenElseTable, leftToken, rightToken);
}

PrioTable PrioTable::Level(const TagTable& subTable, const Array<string>& leftToken,
                           const Array<string>& rightToken) const
{
  return PrioTable(AllocTokens({leftToken, tokens, rightToken}), data, subTable, static_cast<size_t>(leftToken.Count));
}

Tag PrioTable::Relation(const string& newTokenName, const string& recentTokenName) const
{
  return Relation(Index(newTokenName), Index(recentTokenName));
}

p_implementation(PrioTable, Array<string>, DumpData)
{
  if(Count == 0)
    return {HW_D_VALUE(Count)};

  const auto maxlength = tokens.Select<size_t>([](const string& t) { return t.size(); })->Max().Value;
  auto head0 = string() | CastLeft(maxlength);
  head0 += "    ";
  auto head1 = head0;
  string result;
  for(auto i = 0; i < Count; i++)
  {
    auto ii = HWLib::Dump(i + 10000);
    head0 += ii[3];
    head1 += ii[4];
    result += (tokens[i] | CastLeft(maxlength)) + " " + ii.substr(3) + " ";
    for(auto j = 0; j < Count; j++)
      result += string(1, data[i][j].value);
    result += "\n";
  }
  return {"\n" + head0 + "\n" + head1 + "\n" + result};
}

bool PrioTable::Trace = false;


string HWLib::Dump(Array<const Array<const Tag>> data)
{
  string head0 = "\n .- ";
  string head1 = "\n |  ";
  string result;
  for(auto i = 0; i < data.Count; i++)
  {
    auto ii = Dump(i + 10000);
    head0 += ii[3];
    head1 += ii[4];
    result += string(" ") + (ii.substr(3)) + " ";
    for(auto j = 0; j < data.Count; j++)
      result += string(1, data[i][j].value);
    result += "\n";
  }
  return head0 + head1 + "\n" + result | Indent();
};


string HWLib::Dump(const TagTable& data)
{
  const auto count = data.size();
  HW_ASSERT_IS(count, ==, 3);

  char* * d = new char*[3];
  auto i = 0;
  for(auto l : data)
  {
    d[i] = new char[3];
    auto j = 0;
    for(const auto ll : l)
    {
      d[i][j] = ll.value;
      j++;
    };
    i++;
  };

  string head0 = "\n .- ";
  string head1 = "\n |  ";
  string result;
  for(auto i = 0; i < count; i++)
  {
    auto ii = Dump(i + 10000);
    head0 += ii[3];
    head1 += ii[4];
    result += string(" ") + ii.substr(3) + " ";
    for(auto j = 0; j < count; j++)
      result += string(1, d[i][j]);
    result += "\n";
  };

  return head0 + head1 + "\n" + result | Indent();
};


size_t PrioTable::Index(const string& name) const
{
  for(auto i = 0; i < Count; i++)
    if(tokens[i] == name)
      return i;

  for(auto i = 0; i < Count; i++)
    if(tokens[i] == Any)
      return i;

  throw MissingFallbackEntryException();
}

Tag PrioTable::Relation(size_t newIndex, size_t recentIndex) const
{
  HW_ASSERT_IS(tokens[newIndex], !=, PrioTableConst::Start);
  HW_ASSERT_IS(tokens[recentIndex], !=, PrioTableConst::End);
  return data[newIndex][recentIndex];
}

Array<string> PrioTable::AllocTokens(const Array<Array<string>>& tokens)
{
  return tokens.ConvertMany<string>()->ToArray;
}

Array<Array<Tag>> PrioTable::AllocData(size_t count, function<Tag(size_t, size_t)> getData)
{
  return Numbers(count)
         ->Select<Array<Tag>>
         ([=](size_t i)
           {
             return Numbers(count)
                    ->Select<Tag>
                    ([=](size_t j)
                      {
                        return getData(i, j);
                      }
                    )
                    ->ToArray;
           }
         )
         ->ToArray;
}

Tag PrioTable::PrioChar(const Array<Array<Tag>>& base, const TagTable& subTable, size_t leftCount, size_t i,
                        size_t j)
{
  HW_D_LOG_HERE;
  HW_D_FUNCTION(base, subTable, leftCount, i, j);

  auto baseCount = base.Count;
  const auto iGroup = FindGroup(i, {leftCount, baseCount});
  const auto jGroup = FindGroup(j, {leftCount, baseCount});

  if(iGroup == 1 && jGroup == 1)
    return_d(base[i - leftCount][j - leftCount]);

  if(iGroup == 2 && jGroup == 0)
  {
    auto result =
      i - leftCount - baseCount < j
        ? HigherTag
        : LowerTag;
    return_d(result);
  }
  return_d((subTable.begin()[iGroup]).begin()[jGroup]);
};

size_t PrioTable::FindGroup(size_t i, const Array<size_t>& counts)
{
  size_t result = 0;
  for(const auto count : counts)
  {
    i -= count;
    if(i < 0)
      return result;
    result++;
  };
  return result;
}
