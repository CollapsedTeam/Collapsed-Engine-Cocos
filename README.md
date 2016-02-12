# Collapsed Engine
  
**Collapsed Engine** is a multi-platform engine for building MMO games.  
It is based on cocos2d-x (game framework), Jinra (network) and SOCI (database).  
It works on every platform supported by Jinra.
  
**Collapsed Engine Core** is currently being developed as part of the **Collapsed Engine**.
  
**Collapsed Engine Core** is a simpler version of Collapsed Engine.  
It's more like a framework rather than engine, because it contains only basic features shared by every MMO game.
  
## Branches
-   core - contains last stable version of Collapsed Engine Core including latest bug fixes
-   core-next - contains future version of Collapsed Engine Core including new features, tweaks and bug fixes
-	??? - TBA
  
## Build Requirements (Windows)
-   Visual Studio 2013
-   [Cocos 3.10](http://www.cocos2d-x.org/filedown/CocosForWin-v3.10.exe)
-   Highly recommended: [Visual Leak Detector](https://vld.codeplex.com/). Remove VLD_FORCE_ENABLE from project's preprocessors, if you don't want to use it.

## Build Requirements (Linux) [SERVER ONLY]
-   build-essential, cmake, libmysqlclient-dev, pthread, lua, oolua and Soci (oolua and Soci may require self compile)

## Few things to do before start
-   import sql/mysql.sql file to your MySQL database
-   copy config.tpl.ini (Server/Server/Data) and rename to config.ini
-   edit config.ini
-   edit Config.h (Client/Classes)