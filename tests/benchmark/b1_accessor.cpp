// accessorpp library
// Copyright (C) 2022 Wang Qi (wqking)
// Github: https://github.com/wqking/accessorpp
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//   http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "test.h"
#include "accessorpp/accessor.h"

static volatile int intValue = 0;
static volatile int intValue2 = 0;

TEST_CASE("b1, accessor vs get/set directly")
{
	constexpr int iterateCount = 1000 * 1000 * 1000;

	{
		const uint64_t cppTime = measureElapsedTime([iterateCount]() {
			for(int i = 0; i < iterateCount; ++i) {
				intValue2 = intValue;
			}
		});
		accessorpp::Accessor<int> accessor;
		const uint64_t accessorTime = measureElapsedTime([iterateCount, &accessor]() {
			for(int i = 0; i < iterateCount; ++i) {
				intValue2 = accessor;
			}
		});
		std::cout << "Get int value: native = " << cppTime << " accessor = " << accessorTime << std::endl;

	}

	{
		const uint64_t cppTime = measureElapsedTime([iterateCount]() {
			for(int i = 0; i < iterateCount; ++i) {
				intValue = i;
			}
		});
		accessorpp::Accessor<int> accessor;
		const uint64_t accessorTime = measureElapsedTime([iterateCount, &accessor]() {
			for(int i = 0; i < iterateCount; ++i) {
				accessor = i;
			}
		});
		std::cout << "Set int value: native = " << cppTime << " accessor = " << accessorTime << std::endl;

	}
}

