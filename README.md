﻿satpass
====
衛星の軌道を解析し、世界各地での衛星の可視時間を計算する。<br>

 * wsatpass.exe : Windows GUI版
 * satpass.exe : Windows コンソール版
 * satpass.out : Ubuntu コンソール版

![capture](doc/capture.png)<br>

出力例：<br>
HTMLファイル[pass\_info.html](doc/pass_info.html)<br>
TEXTファイル[pass\_info.txt](doc/pass_info.txt)<br>

使い方
----
satpassの操作手順を記載する<br>
wsatpassの使い方は、これをGUIに置き換えること<br>

[TLE.txt](TLE.txt)の更新<br>
------
このファイルにはパスの計算を行う人工衛星のTLEを記載する。<br>
一行目は衛星名、二行目はTLE一行目、三行目はTLE二行目である。<br>
この順番でないと正しく読み込まれないので注意すること。<br>
一列目が'#'の行はコメントとみなされる。また、空白行は無視される。<br>
エンコーディングはUTF-8であること、BOMの有無は問わない。<br>
最新のTLEは[NORAD Two-Line Element Sets Current Data](http://www.celestrak.com/NORAD/elements/)などから入手することができる。

[POS.txt](POS.txt)の更新<br>
------
このファイルには観測地点の経度・緯度を指定する。<br>
複数の観測点を指定することが可能である。<br>
4n+0行目は場所の名前、4n+1行目は経度、4n+2行目は緯度、4n+3行目は海抜である。<br>
この順番でないと正しく読み込まれないので注意すること。<br>
一列目が'#'の行はコメントとみなされる。また、空白行は無視される。<br>
エンコーディングはUTF-8であること、BOMの有無は問わない。<br>
国の位置情報は[世界の首都の位置データ](http://www.amano-tec.com/data/world.html)などから入手することができる。

[EVENT.txt](EVENT.txt)の更新<br>
------
このファイルにはイベント名（衛星のダウンリンクモード等）を指定する。<br>
複数のイベントを指定することが可能である。<br>
一列目が'#'の行はコメントとみなされる。また、空白行は無視される。<br>
エンコーディングはUTF-8であること、BOMの有無は問わない。<br>
このデータはイベントという名前こそついてはいるが、備考として自由に使うことができる。<br>

`satpass.exe`を実行する<br>
------
`satpass.exe`を実行し、いくつかの項目の入力を行う。<br>
上記ファイルの内容に誤りがあった場合にはメッセージが出るので、修正して再度起動する。<br>
ファイルの読み込みに成功した場合、以下の流れで設定を行う。<br>

  1. 入力に用いるタイムゾーンの入力
  ```
  Input 1 / 7, TZ for input, JST ('1') or UT ('0') ?:
  ```
  パスの計算を行う期間の設定の時間表示のタイムゾーンを入力する。<br>
  '1'を入力すればJST、'0'を入力すればUTで入力を行うことができるようになる。<br>
  この設定で影響を受けるのは入力形式のみである。

  2. 出力に用いるタイムゾーンの入力
  ```
  Input 2 / 7, TZ for output, JST ('1') or UT ('0') ?:
  ```
  パスの計算結果の出力の時間表示のタイムゾーンを入力する。<br>
  '1'を入力すればJST、'0'を入力すればUTで出力をするようになる。<br>
  入力に用いるタイムゾーンとは独立である。

  3. 計算開始日時の入力
  ```
  Input 3 / 7, start "year mon day hour min sec" in JST:
  ```
  ```
  Input 3 / 7, start "year mon day hour min sec" in UT:
  ```
  入力に用いるタイムゾーンの入力でJSTを選択していれば、JSTで計算開始日時を設定する（上）。<br>
  入力に用いるタイムゾーンの入力でUTを選択していれば、UTで計算開始日時を設定する（下）。<br>
  空白文字で区切って年、月、日、時、分、秒を入力すること。<br>

  4. 計算期間の入力
  ```
  Input 4 / 7, input days to run (1-21):
  ```
  計算の開始日時から終了日時までの期間を日で指定する。<br>
  時、分、秒まで指定する場合は小数点以下に含めること。<br>
  最低1日、最大21日まで設定することが可能。<br>

  5. イベントの設定
  ```
  Input 5 / 7, do you set mode schedule? Yes ('1') or not ('0'):
  ```
  イベントの時間帯を設定するかどうかを入力する。<br>
  '1'を入力すればイベントの設定を行い、'0'を入力すればイベントの設定はスキップする。<br>
  '1'を入力した場合は以下の手順に従う。<br>
  最初に、イベント開始時間の設定を行う。
  ```
  Input <EVENT> start "year mon day hour min sec" in JST:
  ```
  ```
  Input <EVENT> start "year mon day hour min sec" in UT:
  ```
  \<EVENT\>には[EVENT.txt](EVENT.txt)で設定したイベント名が入る。<br>
  入力に用いるタイムゾーンの入力でJSTを選択していれば、JSTでイベント開始日時を設定する（上）。<br>
  入力に用いるタイムゾーンの入力でUTを選択していれば、UTでイベント開始日時を設定する（下）。<br>
  空白文字で区切って年、月、日、時、分、秒を入力すること。<br>
  次に、イベント実施時間の設定を行う。
  ```
  Input event days:
  ```
  このイベントの開始から終了までの期間を日で指定する。<br>
  時、分、秒まで指定する場合は小数点以下に含めること。<br>
  最後に、イベント期間の追加を行うかどうかの選択を行う。
  ```
  Add other span for <EVENT>? Yes ('1') or not ('0'):
  ```
  このイベントにほかの実施時間を追加指定するかどうかを選択する。<br>
  '1'を選択すれば、上記の手順を繰り返し、別の実施時間を追加することができる。<br>
  '0'を選択し、ほかにイベントがある場合には、そのイベントで上記の手順を繰り返す。<br>
  '0'を選択し、ほかにイベントがない場合には、次の手順である出力ファイル名の設定に進む。<br>

  6. 出力ファイル名の設定
  ```
  Input 6 / 7, output file, text ('0'), html ('1') or none ('2'):
  ```
  出力ファイルの種類を指定する。<br>
  '0'を入力すると、普通のテキストファイル（pass_info.txt）が作成される。<br>
  '1'を入力すると、HTMLファイル（pass_info.html）が作成される。<br>
  '2'を入力すると、ファイルは作成されない。<br>

  7. 最終確認
  ```
  Input 7 / 7, proceed? ('1' or '0'):
  ```
  入力内容にに誤りがないかを確認する。<br>
  '1'を選択すればパスの計算に移るが、'0'を選択すればプログラムは終了する。<br>

注意点
----
 * GUI版であるwsatpassには、一つのイベントにつき期間を4つまでしか設定できない制約がある<br>
   一つのイベントに4つ以上の期間を割り当てたい場合には、同じ名前のイベントを`EVENT.txt`に複数登録すること<br>
 * コンソール版で入力中に方向キーを押すと、入力を正常に読み取れなくなってしまう。<br>
   方向キーではなくバックスペースキーでカーソルを移動させること。<br>

動作環境
----
cl (Windows 7) Version 19.11.25507.1 for x86とgcc (Ubuntu 5.4.0-6ubuntu1~16.04.2) 5.4.0 20160609でビルドを確認した。

ビルドの要求
----
ビルドを行うには、以下の環境のインストールが必要である。

 * [tle v2.1.0](https://github.com/mkaminaga/tle)
 * [sat v1.0.2](https://github.com/mkaminaga/sat)

Windowsで使用する場合には、上記に加え以下の環境が必要

 * [Microsoft Visual Studio Community 2017](https://www.microsoft.com/ja-jp/dev/products/community.aspx)
 * [Windows SDK 8.1](https://developer.microsoft.com/ja-jp/windows/downloads/windows-8-1-sdk)

計算精度について
----
[TEST.md](TEST.md)を参照すること。

ライセンス
----
MITライセンスで公開する。<br>
[LICENSE.md](LICENSE.md)を参照すること。
