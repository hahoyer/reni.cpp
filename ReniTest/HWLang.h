#pragma once

namespace _HWLang
{
    void Pattern();
    void SimpleAndDetailed();
    void Simple();
    void Text();
    void Comment();
    void LineCommentError();
    void CommentError();
    void ParserBaseStructure();

    inline void RunAll()
    {
        Pattern();
        SimpleAndDetailed();
        Simple();
        Text();
        Comment();
        LineCommentError();
        CommentError();
        ParserBaseStructure();
    }
}
