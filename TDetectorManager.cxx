#include "TDetectorManager.hxx"

#include "TKoBRATDCData.hxx"
#include "TKoBRAADCData.hxx"
#include "TKoBRADIGData.hxx"

TDetectorManager *TDetectorManager::instance = NULL;

TDetectorManager::TDetectorManager()
{
}

void TDetectorManager::AddDetectorAna(const char *name, TDetectorAna *ana, DTYPE dtype, const char *bank)
{
    if (anas.find(name) == anas.end())
    {
        anas[name] = ana;
        dtypes[name] = dtype;
        banks[name] = bank;
    }
}

TDetectorAna *TDetectorManager::GetDetectorAna(const char *name)
{
    auto it = anas.find(name);
    if (it == anas.end())
        return NULL;
    else
        return it->second;
}

void TDetectorManager::PrintParametersAll()
{
    for (const auto &pair : anas)
    {
        pair.second->PrintParameters();
    }
}

void TDetectorManager::SetTreeAll()
{
    for (const auto &pair : anas)
    {
        pair.second->SetTree();
    }
}

void TDetectorManager::SetDataAll(TDataContainer &container)
{
    for (const auto &pair : anas)
    {
        switch (dtypes[pair.first])
        {
        case KoBRATDC:
            pair.second->SetData(static_cast<TGenericData *>(container.GetEventData<TKoBRATDCData>(banks[pair.first].c_str())));
            break;
        case KoBRAADC:
            pair.second->SetData(static_cast<TGenericData *>(container.GetEventData<TKoBRAADCData>(banks[pair.first].c_str())));
            break;
        case KoBRADIG:
            pair.second->SetData(static_cast<TGenericData *>(container.GetEventData<TKoBRADIGData>(banks[pair.first].c_str())));
            break;            
        default:
            break;
        }
    }
}

void TDetectorManager::ClearAll()
{
    for (const auto &pair : anas)
    {
        pair.second->Clear();
    }
}

void TDetectorManager::AnalysisAll()
{
    for (const auto &pair : anas)
    {
        pair.second->Analysis();
    }
}

TDetectorManager *TDetectorManager::GetInstance()
{
    if (!instance)
        instance = new TDetectorManager();
    return instance;
}
