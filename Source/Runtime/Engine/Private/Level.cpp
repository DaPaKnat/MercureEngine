#include "PCH.hpp"

#include "Object/Object.hpp"
#include "GameFramework/Entity.hpp"
#include "GameWorldSettings.hpp"
#include "Level.hpp"
#include "ObjectFactory.hpp"

// ==============================[Public Local Methods]============================== //

void	Level::BeginPlay                ()
{
	if (m_owningWorld != nullptr)
	{
		if (m_owningWorld->HasBegunPlay())
		{
			for (auto entity : m_entities)
			{
                entity->BeginPlay();
			}
		}
	}
}

void	Level::Tick		                (float const	    p_deltaTime)
{
	if (m_owningWorld != nullptr)
	{
		if (m_owningWorld->HasBegunPlay())
		{
			for (auto entity : m_entities)
			{
                entity->Tick(p_deltaTime);
			}
		}
	}
}

void	Level::EndPlay	                ()
{
	if (m_owningWorld != nullptr)
	{
		if (m_owningWorld->HasBegunPlay())
		{
			for (auto entity : m_entities)
			{
                entity->EndPlay();
			}
		}
	}
}

Level*  Level::Load                     (std::string const& p_name)
{
    if (CheckContentDirectory() == false)
        return this;

    m_entities.clear();

    std::fstream file;

    file.open(ASSET_DIRECTORY + std::string("/Levels/") + p_name + ".level");

    if (file.is_open())
    {
        Json loader;

        loader = Json::parse(file);

        if (loader.contains("Entities"))
        {
            for (auto const& node : loader["Entities"])
                m_entities.push_back(ObjectFactory::CreateEntityFromTypeID(node, loader["Components"]));
        }
    }

    return this;
}

bool    Level::Save                     ()
{
    if (CheckContentDirectory() == false)
        return false;

    std::ofstream file = std::ofstream(ASSET_DIRECTORY + std::string("/Levels/") + std::string(GetName()) + ".level");
    Json          save;

    for (auto entity : m_entities)
    {
        if (entity != nullptr)
        {
            for (auto component : entity->GetComponents())
            {
                if (component != nullptr)
                {
                    Json& comps = save["Components"];
                    Json& type = comps[std::to_string(component->GetInstanceType())];

                    Json data;

                    component->Serialize(data);

                    type[std::to_string(component->GetInstanceID())] = data;
                }
            }

            entity->Serialize(save["Entities"]);
        }
    }

    file << save.dump(4);

    file.close();

    return true;
}

bool    Level::CheckContentDirectory    ()  const
{
    if (std::filesystem::exists(ASSET_DIRECTORY))
    {
        if (!std::filesystem::exists(ASSET_DIRECTORY + std::string("/Levels/")))
        {
            return std::filesystem::create_directory(ASSET_DIRECTORY + std::string("/Levels/"));
        }

        return true;
    }
    else
        return false;
}