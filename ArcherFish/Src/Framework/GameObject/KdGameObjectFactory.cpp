#include "KdGameObjectFactory.h"

void KdGameObjectFactory::RegisterCreateFunction(const std::string_view _str, const std::function<std::shared_ptr<KdGameObject>(void)> _func)
{
	m_createFunctions[_str.data()] = _func;
}

std::shared_ptr<KdGameObject> KdGameObjectFactory::CreateGameObject(const std::string_view _objName) const
{
	auto creater = m_createFunctions.find(_objName);

	if (creater == m_createFunctions.end())
	{
		assert(0 && "GameObjectFactoryに未登録のゲームオブジェクトクラスです");

		return nullptr;
	}

	return  creater->second();
}