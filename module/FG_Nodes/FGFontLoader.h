#include "StdAfx.h"
#include <CryFlowGraph/IFlowBaseNode.h>
/*
class CFGFontLoader final : public CFlowBaseNode<eNCT_Instanced>
{
public:
    CFGFontLoader(SActivationInfo* pActInfo)
    {

    }

    virtual IFlowNodePtr Clone(SActivationInfo* pActInfo)
    {
        return new CFGFontLoader(pActInfo);
    };

    virtual void GetConfiguration(SFlowNodeConfig& config)
    {
        static const SInputPortConfig in_config[] = {
            InputPortConfig_Void("Get",        _HELP("Get and Load Font")),
            InputPortConfig<string>("String", _HELP("Input the name of font you wish to load (just name no exension)")),
            InputPortConfig<bool>("LoadFont", _HELP("Start retrieving from")),
        { 0 }
        };
        static const SOutputPortConfig out_config[] = {
            OutputPortConfig<string>("FontName"),
            OutputPortConfig<bool>("Start"),
        { 0 }
        };
        config.sDescription = _HELP("Manual start a Font");
        config.pInputPorts = in_config;
        config.pOutputPorts = out_config;
        config.SetCategory(EFLN_APPROVED);
    }

    virtual void ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
    {
        switch (event)
        {
        case eFE_Activate:
            if (IsPortActive(pActInfo, 0))
            {
                string str1 = GetPortString(pActInfo, 1);
                bool activate = GetPortInt(pActInfo, 2);

                if (activate)
                {
                    CryLogAlways("Getting font %s", str1);
                }
                ActivateOutput(pActInfo, 0, str1);
                ActivateOutput(pActInfo, 1, activate);
            }
            break;
        }
    }

    virtual void GetMemoryUsage(ICrySizer* s) const
    {
        s->Add(*this);
    }

private:
    std::shared_ptr<CFGFontLoader> pFontLoader;

};

REGISTER_FLOW_NODE("CryNoesis:CFGFontLoader", CFGFontLoader);*/