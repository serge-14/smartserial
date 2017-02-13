#ifndef _SMART_SERIAL_LIB_
#define _SMART_SERIAL_LIB_

struct StreamInterface;
struct CallbackWrapperInterface;

#define MAX_COMMANDS_PER_NODE 4
#define MAX_COMMAND_SIZE 32
#define MAX_COMMANDS 32
#define MAX_PARAMETERS_PER_COMMAND 4

class SmartSerial
{
public:
    SmartSerial( StreamInterface* pStream );

    bool registerCommand( const char* arrKeywords, CallbackWrapperInterface* pCallback );

    void poll();
private:
    struct node
    {
        node* m_arrChildren[ MAX_COMMANDS_PER_NODE ];
        size_t uChildrenCount;
        char keyword[ MAX_COMMAND_SIZE + 1 ];
        CallbackWrapperInterface* m_pCommand;

        node();
        node( CallbackWrapperInterface* pCommand );
    };

    const char* getNextParameter( const char* strBuffer );
    bool iterate( const char*& strCommand, const char*& strNextCommand );
    bool find( const char* strKeyword, size_t uLength, node* pNode, node*& pFoundNode );

    char m_strCommand[ MAX_COMMAND_SIZE + 1 ];

    StreamInterface* m_pStream;
    size_t m_uUsedCommands;
    node m_nodes[ MAX_COMMANDS ];
    node m_nodeRoot;
};

#endif //_SMART_SERIAL_LIB_