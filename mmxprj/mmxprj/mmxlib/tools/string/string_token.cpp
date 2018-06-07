#include "string_token.h"
#include <cstring>

namespace mmx
{
    namespace tools
    {
        namespace string
        {

            StringToken::StringToken(const char* source) :
                token_(source, source + strlen(source) + 1),
                index_(0)
            {

            }

            StringToken::StringToken(StringToken &&token) :
                token_(std::move(token.token_)),
                index_(token.index_)
            {
                token.index_ = 0;
            }

            StringToken::~StringToken()
            {

            }

            const char *StringToken::GetToken(const char* delim)
            {
                char* rc = nullptr;

                int sz = token_.size();

                if (index_ < sz)
                {

                    rc = token_.data() + index_;

                    if (delim == nullptr || *delim == 0)
                    {
                        index_ = sz;
                    }
                    else
                    {
                        int cut = delims(rc, delim);

                        index_ += cut;
                        rc += cut;

                        if (*rc != 0)
                        {
                            auto ps = rc;

                            while (*ps != 0 && !is_delim(*ps, delim))
                            {
                                ps++;
                            }

                            index_ += ps - rc + 1;
                            *ps = 0;
                        }
                        else
                        {
                            rc = nullptr;
                        }

                    }
                }

                return rc;
            }


            int StringToken::delims(char* source, const char* delim)
            {
                int rc = 0;

                while(*source != 0)
                {
                    if (is_delim(*source, delim))
                    {
                        rc++;
                        source++;
                    }
                    else
                    {
                        break;
                    }
                }

                return rc;
            }

            bool StringToken::is_delim(char c, const char* delim)
            {
                while(*delim != 0)
                {
                    if (*delim++ == c)
                    {
                        return true;
                    }
                }

                return false;
            }

        }//string
    }//tools
}//mmx
