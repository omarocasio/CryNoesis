#include "StdAfx.h"

#include "XamlProvider.h"

#include <NsCore/Ptr.h>
#include "FileProvider.h"
#include <CrySystem/ConsoleRegistration.h>
#include <CrySystem/File/ICryPak.h>

static CXamlProvider* g_pProvider = nullptr;

static void ReloadAllCMD(IConsoleCmdArgs* args)
{
	if (g_pProvider == nullptr)
		return;

	g_pProvider->ReloadAllXaml();
}

static void ReloadCMD(IConsoleCmdArgs* args)
{
	if (args->GetArgCount() < 2 || g_pProvider == nullptr)
		return;

	g_pProvider->ReloadSpecificXaml(args->GetArg(1));
}

static void AddSearchPathCMD(IConsoleCmdArgs* args)
{
	if (args->GetArgCount() < 2 || g_pProvider == nullptr)
		return;

	g_pProvider->AddSearchPath(args->GetArg(1));
}

static void RemoveSearchPathCMD(IConsoleCmdArgs* args)
{
	if (args->GetArgCount() < 2 || g_pProvider == nullptr)
		return;

	g_pProvider->RemoveSearchPath(args->GetArg(1));
}

CXamlProvider::CXamlProvider()
{
	ConsoleRegistrationHelper::AddCommand("Noesis.ReloadAll", &ReloadAllCMD, 0, "Reloads all loaded xamls");
	ConsoleRegistrationHelper::AddCommand("Noesis.Reload", &ReloadCMD, 0, "Reloads a specific loaded xamls");

	ConsoleRegistrationHelper::AddCommand("Noesis.AddSearchPath", &AddSearchPathCMD, 0, "Adds a path to search for xamls in");
	ConsoleRegistrationHelper::AddCommand("Noesis.RemoveSearchPath", &RemoveSearchPathCMD, 0, "Removes a xaml search path");

	g_pProvider = this;
}

CXamlProvider::~CXamlProvider()
{
	g_pProvider = nullptr;
}

Noesis::Ptr<Noesis::Stream> CXamlProvider::LoadXaml(const Noesis::Uri& uri)
{
	std::filesystem::path  path = std::filesystem::u8path(uri.Str()).make_preferred();

	if (!path.has_extension())
		path += std::filesystem::path(".xaml");

	std::string pathString = path.string();

	auto pStream = CPakStream::Open(pathString.c_str());

	if (!pStream.GetPtr())
	{
		std::filesystem::path currentPath;
		for (auto& searchPath : m_searchPaths)
		{
			currentPath = searchPath;
			currentPath /= path;
			pathString = currentPath.string();

			pStream = CPakStream::Open(pathString.c_str());

			if (pStream.GetPtr())
				break;
		}
	}

	if (pStream.GetPtr())
		m_xamls.emplace(uri.Str());

	return pStream;
}

void CXamlProvider::ReloadAllXaml()
{
	for (auto& xaml : m_xamls)
		RaiseXamlChanged(xaml.c_str());
}

void CXamlProvider::ReloadSpecificXaml(const char* uri)
{
	auto xaml = m_xamls.find(uri);
	if (xaml != m_xamls.end())
		RaiseXamlChanged(xaml->c_str());
}

void CXamlProvider::AddSearchPath(const char* uri)
{
	std::filesystem::path  path = std::filesystem::u8path(uri).make_preferred();
	m_searchPaths.emplace(path);
}

void CXamlProvider::RemoveSearchPath(const char* uri)
{
	std::filesystem::path  path = std::filesystem::u8path(uri).make_preferred();
	m_searchPaths.erase(path);
}


class CFGXamlLoader final : public CFlowBaseNode<eNCT_Instanced>
{
public:
    CFGXamlLoader(SActivationInfo* pActInfo){}

    virtual IFlowNodePtr Clone(SActivationInfo* pActInfo)
    {
        return new CFGXamlLoader(pActInfo);
    };

    virtual void GetConfiguration(SFlowNodeConfig& config)
    {
        static const SInputPortConfig in_config[] = {
            InputPortConfig_Void("Get",        _HELP("Get and Load Xaml")),
            InputPortConfig<string>("XamlName", _HELP("Input the name of xmal file you wish to load (just name no exension)")),
        { 0 }
        };
        static const SOutputPortConfig out_config[] = {
            OutputPortConfig<string>("XamlName"),
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
                if (!str1.empty())
                {
                    CryLogAlways("Getting xaml %s", str1);
					g_pProvider->LoadXaml(str1.c_str());
                }
                ActivateOutput(pActInfo, 0, str1);
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

REGISTER_FLOW_NODE("CryNoesis:CFGXamlLoader", CFGXamlLoader);

/////////////////// 
class CFGXamlReloadAll final : public CFlowBaseNode<eNCT_Instanced>
{
public:
    CFGXamlReloadAll(SActivationInfo* pActInfo)
    {

    }

    virtual IFlowNodePtr Clone(SActivationInfo* pActInfo)
    {
        return new CFGXamlReloadAll(pActInfo);
    };

    virtual void GetConfiguration(SFlowNodeConfig& config)
    {
        static const SInputPortConfig in_config[] = {
            InputPortConfig_Void("Get",        _HELP("Reload Xamls")),
            InputPortConfig<bool>("Reload All", _HELP("Reload All Xamls")),
        { 0 }
        };
        static const SOutputPortConfig out_config[] = {
            OutputPortConfig<bool>("Succuss"),
        { 0 }
        };
        config.sDescription = _HELP("Reload all Xamls in data directory");
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
                bool activate = GetPortInt(pActInfo, 1);

                if (activate)
                {
                    g_pProvider->ReloadAllXaml();
                }

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

REGISTER_FLOW_NODE("CryNoesis:CFGXamlReloadAll", CFGXamlReloadAll);

////////////

class CFGReloadSpecificXaml final : public CFlowBaseNode<eNCT_Instanced>
{
public:
    CFGReloadSpecificXaml(SActivationInfo* pActInfo) {}

    virtual IFlowNodePtr Clone(SActivationInfo* pActInfo)
    {
        return new CFGReloadSpecificXaml(pActInfo);
    };

    virtual void GetConfiguration(SFlowNodeConfig& config)
    {
        static const SInputPortConfig in_config[] = {
            InputPortConfig_Void("Get",        _HELP("Reload Specific Xaml")),
            InputPortConfig<string>("String", _HELP("Input the name of xmal file you wish to load (just name no exension)")),
        { 0 }
        };
        static const SOutputPortConfig out_config[] = {
            OutputPortConfig<string>("XamlName"),
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
                if (!str1.empty())
                {
                    CryLogAlways("Getting xaml %s", str1);
                    g_pProvider->LoadXaml(str1.c_str());
                }
                ActivateOutput(pActInfo, 0, str1);
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

REGISTER_FLOW_NODE("CryNoesis:CFGReloadSpecificXaml", CFGReloadSpecificXaml);