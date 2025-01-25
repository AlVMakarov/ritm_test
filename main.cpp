#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

/**
 * @brief Структура для парсинга входного файла
 *
 */
enum ag_func
{
    pmin,
    pmult,
    pcopy_ed,
    pcopy_vert,
    unparsed
};
/**
 * @brief "Хэш-функция"
 * Сопоставляем строке значение из структуры ag_func
 * @param s: входная строка ("min","*","e","v", float)
 * @return ag_func: атрибут из структуры
 */
ag_func hashstr(std::string const &s)
{
    if (s == "min")
        return pmin;
    if (s == "*")
        return pmult;
    if (s == "e")
        return pcopy_ed;
    if (s == "v")
        return pcopy_vert;

    return unparsed;
}

/**
 * @brief Класс для описания узлов и ребер графа
 *
 */
class VE
{
private:
    std::string atr;  // текущее значение в элементе графа
                      // может быть:
                      // - агент функцией - "min","*"
                      // - указателем на элемент, из которого копируем значение
                      // - искомым значением
    bool isv = false; // поле, которое указывает, является ли значение в поле atr искомым

public:
    VE();
    ~VE();
    void calcstat();
    void add(std::string &s);
    float get();
    std::string gets();
    bool getstat();
};
/**
 * @brief Конструктор по умолчанию
 *
 * @return VE
 */
VE ::VE() {}
/**
 * @brief Деструктор
 *
 */
VE::~VE() {}
/**
 * @brief Изменяем статус графа
 * Определяем элемент графа как посчитанный
 */
void VE::calcstat() { isv = true; }
/**
 * @brief Устанавливаем значение в поле atr элемента графа
 *
 * @param s: строка содержащая либо искомое значение, либо агент функцию
 */
void VE::add(std::string &s) { atr = s; };
/**
 * @brief Получаем искомое значение из графа
 *
 * @return float: искомое значение элемента графа
 */
float VE::get()
{
    float val;
    std::stringstream ss(atr);
    ss >> val;
    return val;
}
/**
 * @brief Получаем атрибут в виде строки
 * Недочет кода из-за того, что атрибут хранит как искомое значение,
 * так и указатель на элемент графа, откуда надо копировать
 * @return std::string : atr в виде строки
 */
std::string VE::gets() { return atr; }
/**
 * @brief Статус элемента графа
 * Получаем статус элемента графа, который определяет
 * является ли значение в поле atr искомым или нет
 * @return true
 * @return false
 */
bool VE::getstat() { return isv; }

/**
 * @brief Класс для описания взаимосвязей элементов графа
 *
 */
class MGraph
{
private:
    int NV, NE;                           // Количество узлов и ребер соответственно
    std::vector<std::vector<int>> vertex; // храним указатели на смежные ребра
    std::vector<int> edge;                // храним указатели на смежные вершины

public:
    MGraph(int ne, int nv);
    ~MGraph();
    void addedge(int en, int src, int dst);
    std::vector<int> get_adj_edge(int pi);
    int get_adj_vertex(int pi);
};
/**
 * @brief Construct a new MGraph::MGraph object
 *
 * @param ne : количество ребер
 * @param nv : количество узлов
 */
MGraph::MGraph(int ne, int nv)
{
    NE = ne;
    NV = nv;
    vertex = std::vector<std::vector<int>>(NV);
    edge = std::vector<int>(NE);
}
/**
 * @brief Destroy the MGraph::MGraph object
 *
 */
MGraph::~MGraph() {}
/**
 * @brief Метод для добавления ребра
 *
 * @param en : порядковый номер ребра
 * @param src : корневой узел
 * @param dst : дочерний узел
 */
void MGraph::addedge(int en, int src, int dst)
{
    vertex[dst - 1].push_back(en + NV); // Класс хранит как узлы, так и ребра,
                                        // поэтому указатель на ребра смещается на число узлов
    edge[en] = src - 1;                 // Указатель на смежную вершину ребра
                                        // нумерация в с++ начинается с 0, во входном файле с 1 - учитываем это
}
/**
 * @brief Метод для получения смежных ребер у узла
 *
 * @param pi : указатель на узел
 * @return std::vector<int> : массив указателей на ребра
 */
std::vector<int> MGraph::get_adj_edge(int pi) { return vertex[pi]; }
/**
 * @brief Метод для получения коренного узла для ребра
 *
 * @param pi : указатель на ребро
 * @return int : указатель на узел
 */
int MGraph::get_adj_vertex(int pi) { return edge[pi]; }

/**
 * @brief Агент функция "min"
 * Описывает логику агент-функции "min":
 * получаем смежные к вершине ребра, смотрим их статус, если искомые значения в ребрах найдены,
 * то находим минимальное значение из них, иначе добавляем указатель на ребро в стек и выходим из функции
 * @param p : указатель на вершину, над которой определена функция
 * @param gr : объект MGraph, содержащий информацию об связях в графе
 * @param ve : указатель на массив с элементами графа
 * @param st : указатель на стек
 * @return true : функция отработала нормально
 * @return false : для расчета функции не хватило данных
 */
bool find_min(int p, MGraph &gr, std::vector<VE> &ve, std::vector<int> &st)
{
    int pp1, pp2;
    std::vector<int> earr = gr.get_adj_edge(p); // список смежных ребер узла
    pp1 = earr[0];                              // указатели на ребра
    pp2 = earr[1];                              //
    std::string res;                            // переменная для хранения результата

    /*
    Проверяем смежные ребра - если значение в них не определено,
    то выходим из функции и добавляем указатель на ребро в стек
    */
    if (!ve[pp1].getstat())
    {
        st.push_back(pp1);
        return false;
    }
    else if (!ve[pp2].getstat())
    {
        st.push_back(pp2);
        return false;
    }

    ve[p].calcstat();                               // изменили статус элемента графа на посчитано
    res = std::min(ve[pp1].gets(), ve[pp2].gets()); // значение элемента графа
    ve[p].add(res);                                 // установили в элемент графа значение
    st.pop_back();                                  // убрали узел из стека
    return true;
}
/**
 * @brief Агент-функция "*"
 * Описывает логику агент-функции "*":
 * находит корневую вершину для ребра, определяет для этой вершины
 * указатели на ребра, число которых может быть произвольным. Если какой-то из элементов
 * во время работы функции не определен, то добаваляет указатель на этот элемент в стек
 * и выходит из функции
 * @param NV : число вершин - узлы и вершины находятся в одном массиве, учитываем это
 * @param p : указатель на ребро в массиве вершин и ребер
 * @param gr : объект MGraph, содержащий информацию об связях в графе
 * @param ve : указатель на массив с элементами графа
 * @param st : указатель на стек
 * @return true : функция отработала нормально
 * @return false : для расчета функции не хватило данных
 */
bool find_mult(int NV, int p, MGraph &gr, std::vector<VE> &ve, std::vector<int> &st)
{

    int pp1 = p - NV;                                  // Указатель на ребро - вычитаем из общего массива кол-во узлов
    int ppsrc = gr.get_adj_vertex(pp1);                // Указатель на вершину у ребра
    std::vector<int> edglist = gr.get_adj_edge(ppsrc); // Получаем список смежных ребер
    float res = 1.0;

    /*
        Проверяем смежные ребра и вершину - если значение в них не определено,
        то выходим из функции и добавляем указатель на ребро в стек
    */
    if (ve[ppsrc].getstat()) // проверяем узел
    {
        st.push_back(ppsrc);
        res *= ve[ppsrc].get();
    }
    else
    {
        return false;
    }

    for (auto &el : edglist)
    {
        if (ve[el].getstat())
        {
            st.push_back(el);
            res *= ve[el].get();
        }
        else
        {
            return false;
        }
    }

    std::ostringstream ss; // перевод float в string
    ss << res;             // (недочет кода)
    std::string sres = ss.str();

    ve[p].add(sres);  // установили в элемент графа значение
    ve[p].calcstat(); // поменяли статус элемента графа
    st.pop_back();    // убрали узел из стека
    return true;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cout << "Error: Wrong count of argv. Run '$ main /path/to/input_data /path/to/out_data'\n";
        exit(1);
    }

    char *fin = argv[1];
    char *fout = argv[2];

    std::ifstream in;
    std::ofstream out;

    in.open(fin);
    if (!in.is_open())
    {
        std::cout << "Error: Cannot open input file\n";
        exit(1);
    }
    out.open(fout);

    int NV, NE;
    in >> NV; // Число узлов
    in >> NE; // Число ребер

    MGraph graph(NE, NV);

    int src, dst; // корневой и дочерний узел в ребре

    for (int i = 0; i < NE; i++)
    {
        in >> src;
        in >> dst;
        graph.addedge(i, src, dst); // Добавляем ребро в граф
    }

    std::string s;
    std::vector<VE> ve = std::vector<VE>(NE + NV); // массив из элементов графа
    int buf;

    /*
     * Проходимся по элементам агент-функции из считываемого файла
     */
    for (int i = 0; i < NE + NV; i++)
    {
        in >> s;
        /*
         * Парсим входной файл
         * В случае min, * просто меняем атрибуту элемента графа эту строку
         * Если надо скопировать значение из другого узла/ребра, то добавляем соответсвенный указатель на этот элемент
         * В оставшемся случае помечаем элмент как посчитанный и меняем ззначение атрибута на считанное
         */
        switch (hashstr(s))
        {
        case pmin:
            ve[i].add(s);
            break;
        case pmult:
            ve[i].add(s);
            break;
        case pcopy_ed:
            in >> buf;
            buf = buf + NV - 1;
            s = std::to_string(buf);
            ve[i].add(s);
            break;
        case pcopy_vert:
            in >> buf;
            buf -= 1;
            s = std::to_string(buf);
            ve[i].add(s);
            break;
        default:
            ve[i].calcstat();
            ve[i].add(s);
        }
    }

    std::vector<int> stack; // стек для обхода элементов графа
    int idx = 0;
    bool next; // флаг для преждевременного выхода из расчета элементов стека

    for (auto &el : ve)
    {
        if (!el.getstat())
        {
            stack.push_back(idx);
        }

        next = true;
        /**
         * Цикл для расчета стека
         * Флаг next используется для выхода из цикла в случае
         * если для расчета текущего значение потребуется значение из далее идущего элемента
         */
        while (!stack.empty() and next)
        {

            int p = stack.back();

            if (ve[p].getstat()) // не делаем лишних вычислений
            {
                stack.pop_back();
                continue;
            }

            std::string f = ve[p].gets(); // получаем агент-функцию из не посчитанного элемента графа

            /**
             * Пробуем посчитать элемент графа,
             * иначе добавляем необходимый для расчета элемент в стек и запускаем следующую итерацию цикла
             */
            switch (hashstr(f))
            {
            case pmin: // функция min объявлена только для узлов в задании
                next = find_min(p, graph, ve, stack);
                break;

            case pmult: // функция * объявлена только для ребер в задании
                next = find_mult(NV, p, graph, ve, stack);
                break;

            default:
                /**
                 * Если на вход стека приходит число, проверяем, искомое ли это значение или указатель
                 * Если указатель, то добавляем его в стек и идем на следующую итерацию цикла
                 */
                int ppdst = std::stoi(f);
                if (ve[ppdst].getstat())
                {
                    std::string res = ve[ppdst].gets();
                    ve[p].calcstat();
                    ve[p].add(res);
                    stack.pop_back();
                }
                else
                {
                    stack.push_back(ppdst);
                    break;
                }
                break;
            }
        }
        idx++;
    }

    /**
     * Записываем результат в файл
     */
    for (int i = 0; i < NE + NV; i++)
    {
        out << ve[i].get() << "\n";
    }

    in.close();
    out.close();
}
