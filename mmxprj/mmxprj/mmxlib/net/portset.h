#ifndef _MMX_SNIFF_PORT_SET_H
#define _MMX_SNIFF_PORT_SET_H


namespace mmx
{
	
    namespace net
	{
        static const int PORT_SET_SIZE = 0x10000 / 8;

		class PortSet
		{
            unsigned char set_[PORT_SET_SIZE];
            int count_;

        public:
            PortSet();
            PortSet(const PortSet& portset);
			~PortSet();

            void Set(unsigned short port);
            void Clr(unsigned short port);
            int SetRange(unsigned short begin, unsigned short end, short step = 1);
            int ClrRange(unsigned short begin, unsigned short end, short step = 1);
            bool operator[] (unsigned short port) const;
            void Clear();
        };
	}
}

#endif
