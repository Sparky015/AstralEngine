/**
* @file DataPointInfoPanel.h
* @author Andrew Fagan
* @date 2/16/25
*/

#pragma once

#include "Core/Memory/Tracking/SceneMetricsStorage.h"

namespace Core {

    /**@brief  */
    size_t* GetSelectedDataPointAddress();

    /**@brief  */
    void ShowDataPointInfoPanel(const SceneMetricsStorage& storage, size_t selectedPointIndex);

}
