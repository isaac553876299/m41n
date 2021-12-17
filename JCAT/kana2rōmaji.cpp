

#include <iostream>
#include <fstream>
#include <string>

int main()
{
	std::string hiragana = "あいうえおかきくけこがぎぐげごさしすせそざじずぜぞたちつてとだぢづでどなにぬねのはひふへほばびぶべぼぱぴぷぺぽまみむめもやゆよゃゅょゎらりるれろわゐゑをん";

	std::string katakana = "アイウエオカキクケコガギグゲゴサシスセソザジズゼゾタチツテトダヂヅデドナニヌネノハヒフヘホバビブベボパピプペポマミムメモヤユヨャュョヮラリルレロワヰヱヲン";

	std::string rōmaji[] =
	{
		"a","i","u","e","o",
		"ka","ki","ku","ke","ko","ga","gi","gu","ge","go",
		"sa","shi","su","se","so","za","ji","zu","ze","zo",
		"ta","chi","tsu","te","to","da","dji","dzu","de","do",
		"na","ni","nu","ne","no",
		"ha","hi","fu","he","ho","ba","bi","bu","be","bo","pa","pi","pu","pe","po",
		"ma","mi","mu","me","mo",
		"ya","yu","yo","ya","yu","yo","wa",
		"ra","ri","ru","re","ro",
		"wa","wi","we","wo",
		"n",
	};

	std::ifstream file_in("input.txt");
	std::ofstream file_out("output.txt");

	std::string str1;
	std::string str2;
	while (std::getline(file_in, str1))
	{
		for (char& c : str1)
		{
			if (int i = hiragana.find(c) >= 0)
			{
				str2 += rōmaji[i - 1];
			}
			else str2 += '#';
		}
	}

	std::cout << str2;
	file_out << str2;

	file_in.close();
	file_out.close();

	return 0;
}

std::string sstr[] =
{
	"hiragana"
	"あいうえお",
	"かきくけこ","がぎぐげご","きゃきゅきょくゎ","ぎゃぎゅぎょぐゎ",
	"さしすせそ","ざじずぜぞ","しゃしゅしょ","じゃじゅじょ",
	"たちつてと","だぢづでど","ちゃちゅちょ","ぢゃぢゅぢょ",
	"なにぬねの","にゃにゅにょ",
	"はひふへほ","ばびぶべぼ","ぱぴぷぺぽ","ひゃひゅひょ","びゃびゅびょ","ぴゃぴゅぴょ",
	"まみむめも","みゃみゅみょ",
	"やゆよ","ゃゅょゎ",
	"らりるれろ","りゃりゅりょ",
	"わゐゑを",
	"ん",

	"katakana"
	"アイウエオ",
	"カキクケコ","ガギグゲゴ","キャキュキョクヮ","ギャギュギョグヮ",
	"サシスセソ","ザジズゼゾ","シャシュショ","ジャジュジョ",
	"タチツテト","ダヂヅデド","チャチュチョ","ヂャヂュヂョ",
	"ナニヌネノ","ニャニュニョ",
	"ハヒフヘホ","バビブベボ","パピプペポ","ヒャヒュヒョ","ビャビュビョ","ピャピュピョ",
	"マミムメモ","ミャミュミョ",
	"ヤユヨ","ャュョヮ",
	"ラリルレロ","リャリュリョ",
	"ワヰヱヲ",
	"ン",

	"rōmaji"
	"aiueo",
	"kakikukeko","gagigugego","kyakyukyo","gyagyugyo",
	"sashisuseso","zajizuzezo","shashusho","jajujo",
	"tachitsuteto","dadjidzudedo","chachucho","djadjudjo",
	"naninuneno","nyanyunyo",
	"hahifuheho","babibubebo","papipupepo","hyahyuhyo","byabyubyo","pyapyupyo",
	"mamimumemo","myamyumyo",
	"yayuyo","yayuyowa",
	"rarirurero","ryaryuryo",
	"wawiwewo",
	"n",
};
