module;

#include <winrt/windows.foundation.h>
#include <winrt/windows.ui.xaml.controls.h>

export module Utils.InputBox;

import <string>;

export std::wstring InputBox(std::wstring title) {
    winrt::Windows::UI::Xaml::Controls::ContentDialog dialog;

    winrt::Windows::UI::Xaml::Controls::TextBox input;
    input.AcceptsReturn(true);
    input.Height(64);
    input.Width(300);
    input.Margin(winrt::Windows::UI::Xaml::ThicknessHelper::FromUniformLength(16));

    dialog.Title(winrt::box_value(title));
    dialog.Content(input);

    const auto result = dialog.ShowAsync().get();
    if (result == winrt::Windows::UI::Xaml::Controls::ContentDialogResult::Primary) {
        return input.Text().c_str();
    } else {
        return L"";
    }
}
