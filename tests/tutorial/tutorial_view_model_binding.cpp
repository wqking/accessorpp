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

// Include the head
#include "accessorpp/accessor.h"

#include "tutorial.h"

#include <iostream>
#include <functional>
#include <vector>
#include <string>

// This is very naive implementation of a callback list. DON'T use it in your project.
// You should use the CallbackList class in eventpp library,
// https://github.com/wqking/eventpp
class NaiveCallbackList
{
public:
	using Callback = std::function<void(const std::string &, void *)>;

	void append(const Callback& callback) {
		callbackList.push_back(callback);
	}

	void operator () (const std::string & text, void * data) {
		for(auto& callback : callbackList) {
			callback(text, data);
		}
	}

private:
	std::vector<Callback> callbackList;
};

class Model
{
private:
	struct MyPolicies
	{
		using OnChangedCallback = NaiveCallbackList;
		using CallbackData = void *;
	};

public:
	accessorpp::Accessor<std::string, MyPolicies> text;
};

class EditView
{
public:
	void bindModel(Model* modelToBind) {
		model = modelToBind;
		model->text.onChanged().append([this](const std::string & text, void * data) {
			this->onModelChanged(text, data);
		});
	}

	void userInput(const std::string & text) {
		displayedText = text;
		model->text.set(text, (void *)this);
	}

	const std::string & getDisplayedText() const {
		return displayedText;
	}

private:
	void onModelChanged(const std::string & text, void * data) {
		if(data != this) {
			displayedText = text;
		}
	}

private:
	Model* model;
	std::string displayedText;
};

class LabelView
{
public:
	void bindModel(Model* modelToBind) {
		model = modelToBind;
		model->text.onChanged().append([this](const std::string & text, void * data) {
			this->onModelChanged(text, data);
		});
	}

	const std::string & getDisplayedText() const {
		return displayedText;
	}

private:
	void onModelChanged(const std::string & text, void * /*data*/) {
		displayedText = "Hello, " + text;
	}

private:
	Model* model;
	std::string displayedText;
};

class Application
{
public:
	Application() {
		editView.bindModel(&model);
		labelView.bindModel(&model);
	}

	void run() {
		std::cout << "Running tutorial view model binding" << std::endl;

		std::cout << "User typed 'world'" << std::endl;
		editView.userInput("world");
		std::cout << "Now EditView displays '" << editView.getDisplayedText()
			<< "'. LabelView displays '" << labelView.getDisplayedText() << "'"
			<< std::endl;

		std::cout << "User typed 'accessor is good'" << std::endl;
		editView.userInput("accessor is good");
		std::cout << "Now EditView displays '" << editView.getDisplayedText()
			<< "'. LabelView displays '" << labelView.getDisplayedText() << "'"
			<< std::endl;

		std::cout << "Set text as 'set by code'" << std::endl;
		model.text = "set by code";
		std::cout << "Now EditView displays '" << editView.getDisplayedText()
			<< "'. LabelView displays '" << labelView.getDisplayedText() << "'"
			<< std::endl;
	}

private:
	Model model;
	EditView editView;
	LabelView labelView;
};

TEST_CASE("Tutorial view model binding")
{
	Application app;
	app.run();
}
