#pragma once

namespace _Scanner
{
    void Pattern();
    void SimpleAndDetailed();
    void Simple();
    void Text();
    void Comment();
    void LineCommentError();
    void CommentError();

    inline void RunAll()
    {
        Pattern();
        SimpleAndDetailed();
        Simple();
        Text();
        Comment();
        LineCommentError();
        CommentError();
    }
}
