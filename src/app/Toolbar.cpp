#include "app.hpp"
#include "gui.hpp"
#include "engine.hpp"


namespace rack {


struct NewItem : MenuItem {
	void onAction() {
		gRackWidget->clear();
	}
};

struct OpenItem : MenuItem {
	void onAction() {
		gRackWidget->openDialog();
	}
};

struct SaveItem : MenuItem {
	void onAction() {
		gRackWidget->saveDialog();
	}
};

struct SaveAsItem : MenuItem {
	void onAction() {
		gRackWidget->saveAsDialog();
	}
};

struct FileChoice : ChoiceButton {
	void onAction() {
		Menu *menu = gScene->createMenu();
		menu->box.pos = getAbsolutePos().plus(Vec(0, box.size.y));
		menu->box.size.x = box.size.x;

		{
			MenuItem *newItem = new NewItem();
			newItem->text = "New";
			newItem->rightText = GUI_MOD_KEY_NAME "+N";
			menu->pushChild(newItem);

			MenuItem *openItem = new OpenItem();
			openItem->text = "Open";
			openItem->rightText = GUI_MOD_KEY_NAME "+O";
			menu->pushChild(openItem);

			MenuItem *saveItem = new SaveItem();
			saveItem->text = "Save";
			saveItem->rightText = GUI_MOD_KEY_NAME "+S";
			menu->pushChild(saveItem);

			MenuItem *saveAsItem = new SaveAsItem();
			saveAsItem->text = "Save As";
			saveAsItem->rightText = GUI_MOD_KEY_NAME "+Shift+S";
			menu->pushChild(saveAsItem);
		}
	}
};


struct PauseItem : MenuItem {
	void onAction() {
		gPaused = !gPaused;
	}
};

struct SampleRateItem : MenuItem {
	float sampleRate;
	void onAction() {
		gSampleRate = sampleRate;
		gPaused = false;
	}
};

struct SampleRateChoice : ChoiceButton {
	void onAction() {
		Menu *menu = gScene->createMenu();
		menu->box.pos = getAbsolutePos().plus(Vec(0, box.size.y));
		menu->box.size.x = box.size.x;

		PauseItem *pauseItem = new PauseItem();
		pauseItem->text = gPaused ? "Resume Engine" : "Pause Engine";
		menu->pushChild(pauseItem);

		float sampleRates[] = {44100, 48000, 88200, 96000, 176400, 192000};
		int sampleRatesLen = sizeof(sampleRates) / sizeof(sampleRates[0]);
		for (int i = 0; i < sampleRatesLen; i++) {
			SampleRateItem *item = new SampleRateItem();
			item->text = stringf("%.0f Hz", sampleRates[i]);
			item->sampleRate = sampleRates[i];
			menu->pushChild(item);
		}
	}
	void step() {
		if (gPaused)
			text = "Paused";
		else
			text = stringf("%.0f Hz", gSampleRate);
	}
};


Toolbar::Toolbar() {
	float margin = 5;
	box.size.y = BND_WIDGET_HEIGHT + 2*margin;
	float xPos = 0;

	xPos += margin;
	{
		ChoiceButton *fileChoice = new FileChoice();
		fileChoice->box.pos = Vec(xPos, margin);
		fileChoice->box.size.x = 100;
		fileChoice->text = "File";
		addChild(fileChoice);
		xPos += fileChoice->box.size.x;
	}

	xPos += margin;
	{
		SampleRateChoice *srChoice = new SampleRateChoice();
		srChoice->box.pos = Vec(xPos, margin);
		srChoice->box.size.x = 100;
		addChild(srChoice);
		xPos += srChoice->box.size.x;
	}

	xPos += margin;
	{
		wireOpacitySlider = new Slider();
		wireOpacitySlider->box.pos = Vec(xPos, margin);
		wireOpacitySlider->box.size.x = 150;
		wireOpacitySlider->label = "Cable opacity";
		wireOpacitySlider->precision = 0;
		wireOpacitySlider->unit = "%";
		wireOpacitySlider->setLimits(0.0, 100.0);
		wireOpacitySlider->setDefaultValue(50.0);
		addChild(wireOpacitySlider);
		xPos += wireOpacitySlider->box.size.x;
	}

	xPos += margin;
	{
		wireTensionSlider = new Slider();
		wireTensionSlider->box.pos = Vec(xPos, margin);
		wireTensionSlider->box.size.x = 150;
		wireTensionSlider->label = "Cable tension";
		// wireTensionSlider->unit = "";
		wireTensionSlider->setLimits(0.0, 1.0);
		wireTensionSlider->setDefaultValue(0.5);
		addChild(wireTensionSlider);
		xPos += wireTensionSlider->box.size.x;
	}

	/*
	xPos += margin;
	{
		cpuUsageButton = new RadioButton();
		cpuUsageButton->box.pos = Vec(xPos, margin);
		cpuUsageButton->box.size.x = 100;
		cpuUsageButton->label = "CPU usage";
		addChild(cpuUsageButton);
		xPos += cpuUsageButton->box.size.x;
	}
	*/

	xPos += margin;
	{
		Widget *pluginManager = new PluginManagerWidget();
		pluginManager->box.pos = Vec(xPos, margin);
		addChild(pluginManager);
		xPos += pluginManager->box.size.x;
	}
}

void Toolbar::draw(NVGcontext *vg) {
	bndBackground(vg, 0.0, 0.0, box.size.x, box.size.y);
	bndBevel(vg, 0.0, 0.0, box.size.x, box.size.y);

	Widget::draw(vg);
}


} // namespace rack
