#include "BsScriptPrefabUtility.h"
#include "BsMonoManager.h"
#include "BsMonoClass.h"
#include "BsMonoMethod.h"
#include "BsMonoUtil.h"
#include "BsPrefabUtility.h"
#include "BsScriptSceneObject.h"
#include "BsSceneObject.h"
#include "BsPrefab.h"
#include "BsResources.h"

namespace BansheeEngine
{
	ScriptPrefabUtility::ScriptPrefabUtility(MonoObject* instance)
		:ScriptObject(instance)
	{ }

	void ScriptPrefabUtility::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_BreakPrefab", &ScriptPrefabUtility::internal_breakPrefab);
		metaData.scriptClass->addInternalCall("Internal_ApplyPrefab", &ScriptPrefabUtility::internal_applyPrefab);
		metaData.scriptClass->addInternalCall("Internal_RevertPrefab", &ScriptPrefabUtility::internal_revertPrefab);
		metaData.scriptClass->addInternalCall("Internal_HasPrefabLink", &ScriptPrefabUtility::internal_hasPrefabLink);
	}

	void ScriptPrefabUtility::internal_breakPrefab(ScriptSceneObject* nativeInstance)
	{
		if (ScriptSceneObject::checkIfDestroyed(nativeInstance))
			return;

		nativeInstance->getNativeSceneObject()->breakPrefabLink();
	}

	void ScriptPrefabUtility::internal_applyPrefab(ScriptSceneObject* nativeInstance)
	{
		if (ScriptSceneObject::checkIfDestroyed(nativeInstance))
			return;

		String prefabLinkUUID = nativeInstance->getNativeSceneObject()->getPrefabLink();
		HPrefab prefab = static_resource_cast<Prefab>(gResources().loadFromUUID(prefabLinkUUID, false, false));

		if (prefab != nullptr)
			prefab->update(nativeInstance->getNativeSceneObject());

		gResources().save(prefab);
	}

	void ScriptPrefabUtility::internal_revertPrefab(ScriptSceneObject* nativeInstance)
	{
		if (ScriptSceneObject::checkIfDestroyed(nativeInstance))
			return;

		PrefabUtility::revertToPrefab(nativeInstance->getNativeSceneObject());
	}

	bool ScriptPrefabUtility::internal_hasPrefabLink(ScriptSceneObject* nativeInstance)
	{
		if (ScriptSceneObject::checkIfDestroyed(nativeInstance))
			return false;

		return !nativeInstance->getNativeSceneObject()->getPrefabLink().empty();
	}
}