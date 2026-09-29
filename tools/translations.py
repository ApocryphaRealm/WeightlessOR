"""Writes the eleven translation files (rule 66): UTF-16LE with a BOM, "$WL_<Key><TAB>text" per line, into
dist/.../OBSE/Plugins/ApocryphaMenuFramework/Translations/Weightless_<language>.txt. Placeholders (%u) stay as they are.
    python tools/translations.py"""
import os

HERE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OUT = os.path.join(HERE, "dist", "OblivionRemastered", "Binaries", "Win64", "OBSE", "Plugins", "ApocryphaMenuFramework", "Translations")

KEYS = ["WL_Intro", "WL_GroupReading", "WL_Books", "WL_Scrolls", "WL_GroupAlchemy", "WL_Potions", "WL_Food", "WL_Ingredients",
        "WL_Apparatus", "WL_GroupMagic", "WL_SoulGems", "WL_SigilStones", "WL_GroupEveryday", "WL_Misc", "WL_Keys", "WL_Arrows",
        "WL_Lights", "WL_GroupWorn", "WL_Jewelry", "WL_Clothing", "WL_Armor", "WL_Weapons", "WL_ItemCount", "WL_Reset",
        "WL_Waiting", "WL_Status"]

T = {
    "english": ["Items in a category that is switched on weigh nothing in your inventory. Changes apply at once.",
                "Reading", "Books", "Scrolls", "Alchemy", "Potions", "Food and drink", "Ingredients", "Alchemy apparatus",
                "Magic", "Soul gems", "Sigil stones", "Everyday items", "Miscellaneous (gems, ores, clutter)", "Keys", "Arrows",
                "Torches", "Worn and wielded", "Rings and amulets", "Clothing", "Armour", "Weapons", "(%u items)",
                "Reset to the defaults", "Waiting for the game's items to load.", "%u items weigh nothing."],
    "french": ["Les objets d'une catégorie activée ne pèsent rien dans votre inventaire. Les changements s'appliquent aussitôt.",
               "Lecture", "Livres", "Parchemins", "Alchimie", "Potions", "Nourriture et boissons", "Ingrédients", "Matériel d'alchimie",
               "Magie", "Pierres d'âme", "Pierres cachetées", "Objets du quotidien", "Divers (gemmes, minerais, bric-à-brac)", "Clés", "Flèches",
               "Torches", "Portés et maniés", "Anneaux et amulettes", "Vêtements", "Armures", "Armes", "(%u objets)",
               "Rétablir les valeurs par défaut", "En attente du chargement des objets du jeu.", "%u objets ne pèsent rien."],
    "german": ["Gegenstände einer eingeschalteten Kategorie wiegen in Eurem Inventar nichts. Änderungen gelten sofort.",
               "Lesen", "Bücher", "Schriftrollen", "Alchemie", "Tränke", "Essen und Trinken", "Zutaten", "Alchemiegeräte",
               "Magie", "Seelensteine", "Siegelsteine", "Alltagsgegenstände", "Verschiedenes (Edelsteine, Erze, Krimskrams)", "Schlüssel", "Pfeile",
               "Fackeln", "Getragen und geführt", "Ringe und Amulette", "Kleidung", "Rüstung", "Waffen", "(%u Gegenstände)",
               "Auf die Standardwerte zurücksetzen", "Warte, bis die Gegenstände des Spiels geladen sind.", "%u Gegenstände wiegen nichts."],
    "italian": ["Gli oggetti di una categoria attivata non pesano nulla nell'inventario. Le modifiche si applicano subito.",
                "Lettura", "Libri", "Pergamene", "Alchimia", "Pozioni", "Cibi e bevande", "Ingredienti", "Strumenti alchemici",
                "Magia", "Gemme dell'anima", "Pietre dei sigilli", "Oggetti di uso comune", "Varie (gemme, minerali, cianfrusaglie)", "Chiavi", "Frecce",
                "Torce", "Indossati e impugnati", "Anelli e amuleti", "Abiti", "Armature", "Armi", "(%u oggetti)",
                "Ripristina i valori predefiniti", "In attesa che gli oggetti del gioco siano caricati.", "%u oggetti non pesano nulla."],
    "spanish": ["Los objetos de una categoría activada no pesan nada en tu inventario. Los cambios se aplican al instante.",
                "Lectura", "Libros", "Pergaminos", "Alquimia", "Pociones", "Comida y bebida", "Ingredientes", "Aparatos de alquimia",
                "Magia", "Gemas del alma", "Piedras de sigilo", "Objetos cotidianos", "Varios (gemas, minerales, trastos)", "Llaves", "Flechas",
                "Antorchas", "Vestidos y empuñados", "Anillos y amuletos", "Ropa", "Armadura", "Armas", "(%u objetos)",
                "Restablecer los valores predeterminados", "Esperando a que se carguen los objetos del juego.", "%u objetos no pesan nada."],
    "polish": ["Przedmioty z włączonej kategorii nic nie ważą w ekwipunku. Zmiany działają od razu.",
               "Lektura", "Książki", "Zwoje", "Alchemia", "Mikstury", "Jedzenie i picie", "Składniki", "Przyrządy alchemiczne",
               "Magia", "Klejnoty dusz", "Kamienie pieczęci", "Przedmioty codzienne", "Różne (klejnoty, rudy, drobiazgi)", "Klucze", "Strzały",
               "Pochodnie", "Noszone i dzierżone", "Pierścienie i amulety", "Ubrania", "Pancerz", "Broń", "(%u przedmiotów)",
               "Przywróć ustawienia domyślne", "Czekam, aż przedmioty gry się wczytają.", "%u przedmiotów nic nie waży."],
    "czech": ["Předměty ze zapnuté kategorie ve vašem inventáři nic neváží. Změny platí okamžitě.",
              "Čtení", "Knihy", "Svitky", "Alchymie", "Lektvary", "Jídlo a pití", "Přísady", "Alchymistické nástroje",
              "Magie", "Kameny duší", "Pečetní kameny", "Běžné předměty", "Různé (drahokamy, rudy, harampádí)", "Klíče", "Šípy",
              "Pochodně", "Nošené a držené", "Prsteny a amulety", "Oblečení", "Zbroj", "Zbraně", "(%u předmětů)",
              "Obnovit výchozí nastavení", "Čekám na načtení předmětů hry.", "%u předmětů nic neváží."],
    "russian": ["Предметы включённой категории ничего не весят в инвентаре. Изменения действуют сразу.",
                "Чтение", "Книги", "Свитки", "Алхимия", "Зелья", "Еда и напитки", "Ингредиенты", "Алхимические приборы",
                "Магия", "Камни душ", "Камни печатей", "Повседневные предметы", "Разное (самоцветы, руда, хлам)", "Ключи", "Стрелы",
                "Факелы", "Надеваемое и оружие в руках", "Кольца и амулеты", "Одежда", "Доспехи", "Оружие", "(предметов: %u)",
                "Вернуть значения по умолчанию", "Ожидание загрузки предметов игры.", "Ничего не весят предметов: %u."],
    "japanese": ["オンにしたカテゴリーのアイテムは、インベントリで重さがなくなります。変更はすぐに反映されます。",
                 "読み物", "本", "スクロール", "錬金術", "薬", "飲食物", "材料", "錬金器具",
                 "魔法", "魂石", "シジルストーン", "日用品", "その他（宝石、鉱石、がらくた）", "鍵", "矢",
                 "たいまつ", "装備品", "指輪とアミュレット", "衣服", "防具", "武器", "（%u 個）",
                 "初期設定に戻す", "ゲームのアイテムの読み込みを待っています。", "%u 個のアイテムの重さがありません。"],
    "korean": ["켜 둔 분류의 아이템은 소지품에서 무게가 없습니다. 변경 사항은 바로 적용됩니다.",
               "읽을거리", "책", "두루마리", "연금술", "물약", "음식과 음료", "재료", "연금술 도구",
               "마법", "영혼석", "시길 스톤", "일상 용품", "기타 (보석, 광석, 잡동사니)", "열쇠", "화살",
               "횃불", "착용 및 사용 장비", "반지와 목걸이", "의복", "방어구", "무기", "(%u개)",
               "기본값으로 되돌리기", "게임 아이템을 불러오는 중입니다.", "무게가 없는 아이템 %u개."],
    "chinese": ["开启的类别中的物品在物品栏中没有重量。更改立即生效。",
                "读物", "书籍", "卷轴", "炼金", "药水", "食物和饮料", "材料", "炼金器具",
                "魔法", "灵魂石", "符印石", "日常物品", "杂项（宝石、矿石、杂物）", "钥匙", "箭矢",
                "火把", "穿戴与手持", "戒指和护身符", "服装", "护甲", "武器", "（%u 件）",
                "恢复默认设置", "正在等待游戏物品加载。", "%u 件物品没有重量。"],
}

os.makedirs(OUT, exist_ok=True)
for lang, texts in T.items():
    assert len(texts) == len(KEYS), lang
    for k, t in zip(KEYS, texts):
        assert t.count("%u") == T["english"][KEYS.index(k)].count("%u"), (lang, k)
    body = "\r\n".join(f"${k}\t{t}" for k, t in zip(KEYS, texts)) + "\r\n"
    with open(os.path.join(OUT, f"Weightless_{lang}.txt"), "wb") as f:
        f.write(b"\xff\xfe" + body.encode("utf-16-le"))
print(f"{len(T)} languages x {len(KEYS)} keys written to {OUT}")
