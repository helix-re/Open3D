// ----------------------------------------------------------------------------
// -                        Open3D: www.open3d.org                            -
// ----------------------------------------------------------------------------
// The MIT License (MIT)
//
// Copyright (c) 2015 Qianyi Zhou <Qianyi.Zhou@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
// ----------------------------------------------------------------------------

#include "ColorMap.h"

#include <Core/Utility/Console.h>

namespace three{

namespace {

class GlobalColorMapSingleton
{
private:
	GlobalColorMapSingleton() : color_map_(new ColorMapJet) {
		PrintDebug("Global colormap init.\n");
	}
	GlobalColorMapSingleton(const GlobalColorMapSingleton &) = delete;
	GlobalColorMapSingleton &operator=(const GlobalColorMapSingleton &) = 
			delete;
public:
	~GlobalColorMapSingleton() {
		PrintDebug("Global colormap destruct.\n");
	}

public:
	static GlobalColorMapSingleton &GetInstance() {
		static GlobalColorMapSingleton singleton;
		return singleton;
	}

public:
	std::shared_ptr<const ColorMap> color_map_;
};

}	// unnamed namespace

Eigen::Vector3d ColorMapGray::GetColor(double value) const
{
	return Eigen::Vector3d(value, value, value);
}

Eigen::Vector3d ColorMapJet::GetColor(double value) const
{
	return Eigen::Vector3d(
			JetBase(value * 2.0 - 1.5),		// red
			JetBase(value * 2.0 - 1.0),		// green
			JetBase(value * 2.0 - 0.5));	// blue
}

Eigen::Vector3d ColorMapSummer::GetColor(double value) const
{
	return Eigen::Vector3d(
			Interpolate(value, 0.0, 0.0, 1.0, 1.0),
			Interpolate(value, 0.5, 0.0, 1.0, 1.0),
			0.4);
}

Eigen::Vector3d ColorMapWinter::GetColor(double value) const
{
	return Eigen::Vector3d(
			0.0,
			Interpolate(value, 0.0, 0.0, 1.0, 1.0),
			Interpolate(value, 1.0, 0.0, 0.5, 1.0));
}

const std::shared_ptr<const ColorMap> GetGlobalColorMap()
{
	return GlobalColorMapSingleton::GetInstance().color_map_;
}

void SetGlobalColorMap(ColorMap::ColorMapOption option)
{
	switch (option) {
	case ColorMap::COLORMAP_GRAY:
		GlobalColorMapSingleton::GetInstance().color_map_.reset(
				new ColorMapGray);
		break;
	case ColorMap::COLORMAP_SUMMER:
		GlobalColorMapSingleton::GetInstance().color_map_.reset(
				new ColorMapSummer);
		break;
	case ColorMap::COLORMAP_WINTER:
		GlobalColorMapSingleton::GetInstance().color_map_.reset(
				new ColorMapWinter);
		break;
	case ColorMap::COLORMAP_JET:
	default:
		GlobalColorMapSingleton::GetInstance().color_map_.reset(
				new ColorMapJet);
		break;
	}
}

}	// namespace three