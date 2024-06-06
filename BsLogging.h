#ifndef BS_LOGGING_H
#define BS_LOGGING_H

namespace Bs
{
    template<typename ... Ts>
    inline std::string format(const char* fmt, const Ts & ...args)
    {
        std::vector<char> str(1024);
        std::size_t limit = 1 << 24;
        while(true)
        {
            std::size_t n = snprintf(str.data(),str.size(),fmt,args...);
            if(str[str.size()-1] == '\0' && str[str.size()-2] != '\0' && n < limit)
            {
                str.resize(str.size() * 2);
                continue;
            }
            break;
        }
        std::string ret = str.data();
        return ret;
    }

    template<Bs::Literal file>
    struct LogAgent
    {
        
        static constexpr const char * fileName = file.get();

        static std::mutex & getMtx()
        {
            static std::mutex mtx;
            return mtx;
        }

        static bool & getIsInit()
        {
            static bool init = false;
            return init;
        }

        template<typename ... Ts>
        static void writePartial(const char * fmt, const Ts & ...args)
        {
#ifdef BS_ENABLE_LOGGING
            std::lock_guard<std::mutex> lock(getMtx());
            
            const std::string s = Bs::format(fmt,args...);
            std::ofstream myfile;
            if(!getIsInit())
            {
                myfile.open(fileName);
                getIsInit() = true;
            }
            else
            {
                myfile.open(fileName, std::ios_base::app);
            }
            myfile << s.c_str();
            myfile.close();
#endif
        }

        template<typename ... Ts>
        static void write(const char * fmt, const Ts & ...args)
        {
            writePartial(fmt,args...);
            writePartial("\n");
        }

    };

    using LogBS = Bs::LogAgent<Bs::label("BSLog.txt")>;
}
#endif