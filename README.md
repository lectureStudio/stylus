## Stylus pen input for Java

Java native interface implementation for pressure-sensitive input devices.

```xml
<dependency>
    <groupId>org.lecturestudio.stylus</groupId>
    <artifactId>stylus</artifactId>
    <version>0.1.0-SNAPSHOT</version>
</dependency>
```

Swing                      |  JavaFX
:-------------------------:|:-------------------------:
![lecturePresenter Screenshot](doc/readme/stylus-swing.png)  |  ![lectureEditor Screenshot](doc/readme/stylus-javafx.png)

### Supported Platforms
Maven Central artifacts contain native libraries that can be loaded on the following platforms:

| Operating System | Artifact ID                |
| ---------------- |----------------------------|
| Linux            | stylus-linux-x86_64        |
| macOS            | stylus-macos-x86_64        |
| Windows          | stylus-windows-x86_64      |

### Build Notes

In order to build the native code, be sure to install the prerequisite software:

<table>
  <tr>
    <td>Linux</td>
    <td>gcc, g++, libstdc++, libX11-dev, libxi-dev (e.g. for Ubuntu, names may differ depending on your distro)</td>
  </tr>
  <tr>
    <td>macOS</td>
    <td>Xcode 9 or higher</td>
  </tr>
  <tr>
    <td>Windows</td>
    <td>Visual Studio 2017 or higher</td>
  </tr>
</table>

Assuming you have all the prerequisites installed for your OS, run:

```
mvn install
```
