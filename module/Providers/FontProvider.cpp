#include "StdAfx.h"

#include "FontProvider.h"
#include "FileProvider.h"
#include <NsCore/Ptr.h>

#include <CryFlowGraph/IFlowBaseNode.h>



void CFontProvider::ScanFolder(const Noesis::Uri& folder)
{
	CryLogAlways("%s", folder.Str());

	ScanFolder(folder, "otf");
	ScanFolder(folder, "ttf");
	ScanFolder(folder, "ttc");
}

Noesis::Ptr<Noesis::Stream> CFontProvider::OpenFont(const Noesis::Uri& folder, const char* filename) const
{
	stack_string path;
	const char* rootPath = "."; //Make cvar
	path.Format("%s/%s/%s", rootPath, folder, filename);

	return CPakStream::Open(path);
}

void CFontProvider::ScanFolder(const Noesis::Uri& folder, const char* ext)
{
	stack_string path;
	const char* rootPath = "."; //Make cvar
	path.Format("%s/%s/*.%s/", rootPath, folder.Str(), ext);

	_finddata_t findData;
	auto handle = gEnv->pCryPak->FindFirst(path.c_str(), &findData);
	if (handle != -1)
	{
		do
		{
			RegisterFont(folder, findData.name);

		} while (gEnv->pCryPak->FindNext(handle, &findData) != -1);

		gEnv->pCryPak->FindClose(handle);
	}
}



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
                    //CFontProvider::OpenFont(str1.c_str());
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

};

REGISTER_FLOW_NODE("CryNoesis:CFGFontLoader", CFGFontLoader);
