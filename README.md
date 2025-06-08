# comment_tool_gui_qt

The GUI client to the comment_tool
Graphical utility for commenting directories, frontend to console utility comment_tool

# Launching

Should be launched with the first argument in the form of an absolute path to the directory for which the comment is read. After launching, you can select another directory.

# Viewing comments

When the directory is not specified explicitly, the program's working directory is used.
The GUI tries to get comment when app started using comment_tool. 
The console utility is used to read about the selected directory. 
When reading, the first two lines will be discarded as explanations of the console utility output.

# Saving comments

You can write comments to the selected directory. The GUI will call comment_tool to save comment.

# The console app

[C++ backend tool for commenting](https://github.com/ArtNazarov/comment_tool) 

# How to add to KDE6 context menu

Create new file comment_tool_gui_qt.desktop

```
nano ~/.local/share/kio/servicemenus/comment_tool_gui_qt.desktop
```
with the content
```
[Desktop Entry]
Type=Service
MimeType=inode/directory;
Actions=open_with_some_app
X-KDE-StartupNotify=false
X-KDE-Submenu=Открыть с помощью

[Desktop Action open_with_some_app]
Name=Comment directory with comment_tool_gui_qt
Exec=/usr/bin/comment_tool_gui_qt "%f"
Icon=system-run
```
and grant executable permission:
```
chmod +x ~/.local/share/kio/servicemenus/comment_tool_gui_qt.desktop
```
Reload Dolphin menus
```
kbuildsycoca6
```

Note: comment_tool_gui_qt should be copied to path /usr/bin/comment_tool_gui_qt or available as link
