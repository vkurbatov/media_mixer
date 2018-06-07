#ifndef _MMX_TOOLS_STRING_TOKEN_H
#define _MMX_TOOLS_STRING_TOKEN_H

#include <vector>

namespace mmx
{
    namespace tools
    {
        namespace string
        {
            class StringToken
            {
                std::vector<char>       token_;
                int                     index_;

            public:
                StringToken(const char* source);
                StringToken(StringToken&& token);
                ~StringToken();

                const char* GetToken(const char *delim = nullptr);
            private:
                static int delims(char* source, const char* delim);
                static bool is_delim(char c, const char* delim);

            };
        }
    }
}

#endif
