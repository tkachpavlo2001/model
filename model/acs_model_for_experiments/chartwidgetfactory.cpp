#include "chartwidgetfactory.h"
#include "default_configuration_setter.h"
#include "QDebug"

void aDataset_config_base::to_set_config(config&p) { _p_config = p.p_config; }

aDataset_config_base::~aDataset_config_base() {}

iChartWidget::~iChartWidget() {}

iChartWidgetConfig::~iChartWidgetConfig() {}

iWidgetAbstractFactory::~iWidgetAbstractFactory() {}


void iChartWidget::_to_reset_chart()
{
    if (_pSeries != nullptr && _pSeries->count() != 0)
    {
        auto p = _pSeries->at(_pSeries->count()-1);
        p.setX(0);
        _pSeries->clear();
        _pSeries->append(p);
    }
    if (_pSeries_def != nullptr && _pSeries_def->count() != 0)
    {
        auto p = _pSeries_def->at(_pSeries_def->count()-1);
        p.setX(0);
        _pSeries_def->clear();
        _pSeries_def->append(p);
    }
}

void iChartWidget::_to_model_init()
{
/*
    //_pRegistrator = Registrator_qt::to_new(this);
    _pRegistrator->to_set_series(_pSeries);

    const Default_configuration_setter default_configuration_setter_obj;
    // reference case
    ///_definder = std::make_shared<Reference_signal_definder_static>();
    ///_regulator = std::make_shared<PID_regulator>();
    _source = std::make_shared<DC_source_inerted>();
    _process = std::make_shared<DC_engine>();

    default_configuration_setter_obj.to_set_elements_parameters(
                _definder,
                _regulator,
                _source,
                _process
                );

    _process->to_set_calculation_mode(DC_engine::EULER);

    _acs_model = std::make_shared<Automated_control_system_paralleled>();
    _acs_model->to_mount_the_element(_definder.get());
    _acs_model->to_mount_the_element(_regulator.get());
    _acs_model->to_mount_the_element(_source.get());
    _acs_model->to_mount_the_element(_process.get());


    _experiment = std::make_shared<Experiment_executor>();
    _experiment->to_get_model_to_run(_acs_model.get());
    default_configuration_setter_obj.to_set_experiment_parameters(_experiment);
    //set

    double load = 20;
    double inertia = 0.05;
    auto arr = _process->to_get_parameters();
    arr[DC_engine::LOAD_K_0] = 1 * load;
    arr[DC_engine::LOAD_K_1] = 0 * load;
    arr[DC_engine::LOAD_K_EXP_CURVATURE] = 0 * load;
    arr[DC_engine::LOAD_K_EXP_LIMIT] = 0 * load;

    arr[DC_engine::MOMENT_OF_INERTIA_OF_MECHANICAL_LOAD] = 99 * inertia;
    _process->to_set_all_parameters(arr);

    _experiment->to_set_registrator(_pRegistrator);
    //alg
    ///_regulator->to_set_koefficients(3);
    ///_definder->to_set_signal(20);
    _source->to_receive_input_signal(20);
*/


    //_pRegistrator = Registrator_qt::to_new(this);
    _pRegistrator->to_set_series(_pSeries);

    _source = std::make_shared<DC_source_inerted>();

    _acs_model = std::make_shared<Automated_control_system_paralleled>();

    _experiment = std::make_shared<Experiment_executor>();
    _experiment->to_get_model_to_run(_acs_model.get());

}

void ChartWidget_velocity::_to_model_init()
{
    iChartWidget::_to_model_init();

    const Default_configuration_setter default_configuration_setter_obj;

    _process = std::make_shared<DC_engine_velocity_out>();
    _process->to_set_calculation_mode(DC_engine::EULER);

    default_configuration_setter_obj.to_set_elements_parameters(
                _definder,
                _regulator,
                _source,
                _process
                );

    _acs_model->to_mount_the_element(_definder.get());
    _acs_model->to_mount_the_element(_regulator.get());
    _acs_model->to_mount_the_element(_source.get());
    _acs_model->to_mount_the_element(_process.get());


    default_configuration_setter_obj.to_set_experiment_parameters(_experiment);

    _experiment->to_set_t_length(run_time);

    _to_init_model_config();
}
void ChartWidget_velocity::_to_init_model_config()
{    //set

    double load = 20;
    double inertia = 0.05;
    auto arr = _process->to_get_parameters();
    arr[DC_engine::LOAD_K_0] = 1 * load;
    arr[DC_engine::LOAD_K_1] = 0 * load;
    arr[DC_engine::LOAD_K_EXP_CURVATURE] = 0 * load;
    arr[DC_engine::LOAD_K_EXP_LIMIT] = 0 * load;

    arr[DC_engine::MOMENT_OF_INERTIA_OF_MECHANICAL_LOAD] = 99 * inertia;
    _process->to_set_all_parameters(arr);

    _experiment->to_set_registrator(_pRegistrator);
    //alg
    ///_regulator->to_set_koefficients(3);
    ///_definder->to_set_signal(20);
    _source->to_receive_input_signal(50);

    _pAxisX->setTitleText("time (sec)");
    _pAxisY->setTitleText("angular velocity (rad/sec), voltage (V)");
    _pSeries->setName("ω(t)");
    _pSeries_def->setName("V(t)");
}
void ChartWidget_velocity::_to_run()
{

    _to_reset_chart();

    _pMainChart->update();

    qDebug() << "DONE1\n";


    _experiment->to_set_registrator(_pRegistrator);
    _pRegistrator->to_set_dt_to_plot(_experiment->to_check_time_to_show());
    _experiment->to_run();

    qDebug() << "DONE0\n";
}

void ChartWidget_theta::_to_model_init()
{
    iChartWidget::_to_model_init();

    const Default_configuration_setter default_configuration_setter_obj;

    _process = std::make_shared<DC_engine>();
    _process->to_set_calculation_mode(DC_engine::EULER);

    default_configuration_setter_obj.to_set_elements_parameters(
                _definder,
                _regulator,
                _source,
                _process
                );

    _acs_model->to_mount_the_element(_definder.get());
    _acs_model->to_mount_the_element(_regulator.get());
    _acs_model->to_mount_the_element(_source.get());
    _acs_model->to_mount_the_element(_process.get());

    default_configuration_setter_obj.to_set_experiment_parameters(_experiment);

    _experiment->to_set_t_length(run_time);

    _to_init_model_config();
}
void ChartWidget_theta::_to_init_model_config()
{
    //set

    double load = 20;
    double inertia = 0.05;
    auto arr = _process->to_get_parameters();
    arr[DC_engine::LOAD_K_0] = 1 * load;
    arr[DC_engine::LOAD_K_1] = 0 * load;
    arr[DC_engine::LOAD_K_EXP_CURVATURE] = 0 * load;
    arr[DC_engine::LOAD_K_EXP_LIMIT] = 0 * load;

    arr[DC_engine::MOMENT_OF_INERTIA_OF_MECHANICAL_LOAD] = 99 * inertia;
    _process->to_set_all_parameters(arr);

    _experiment->to_set_registrator(_pRegistrator);
    //alg
    ///_regulator->to_set_koefficients(3);
    ///_definder->to_set_signal(20);
    _source->to_receive_input_signal(20);

    _pAxisX->setTitleText("time (sec)");
    _pAxisY->setTitleText("angular displacement (rad), voltage (V)");
    _pSeries->setName("θ(t)");
    _pSeries_def->setName("V(t)");

}
void ChartWidget_theta::_to_run()
{

    _to_reset_chart();

    _pMainChart->update();

    qDebug() << "DONE1\n";


    _experiment->to_set_registrator(_pRegistrator);
    _pRegistrator->to_set_dt_to_plot(_experiment->to_check_time_to_show());
    _experiment->to_run();

    qDebug() << "DONE0\n";
}

void ChartWidget_regulator::_to_model_init()
{
    iChartWidget::_to_model_init();

    const Default_configuration_setter default_configuration_setter_obj;

    // reference case
    _definder = std::make_shared<Reference_signal_definder_static>();
    _regulator = std::make_shared<PID_regulator>();
    _process = std::make_shared<DC_engine>();

    _process->to_set_calculation_mode(DC_engine::EULER);

    default_configuration_setter_obj.to_set_elements_parameters(
                _definder,
                _regulator,
                _source,
                _process
                );

    _acs_model->to_mount_the_element(_definder.get());
    _acs_model->to_mount_the_element(_regulator.get());
    _acs_model->to_mount_the_element(_source.get());
    _acs_model->to_mount_the_element(_process.get());

    default_configuration_setter_obj.to_set_experiment_parameters(_experiment);

    _experiment->to_set_t_length(run_time);

    _to_init_model_config();
}
void ChartWidget_regulator::_to_init_model_config()
{
    //set

    double load = 20;
    double inertia = 0.05;
    auto arr = _process->to_get_parameters();
    arr[DC_engine::LOAD_K_0] = 1 * load;
    arr[DC_engine::LOAD_K_1] = 0 * load;
    arr[DC_engine::LOAD_K_EXP_CURVATURE] = 0 * load;
    arr[DC_engine::LOAD_K_EXP_LIMIT] = 0 * load;

    arr[DC_engine::MOMENT_OF_INERTIA_OF_MECHANICAL_LOAD] = 99 * inertia;
    _process->to_set_all_parameters(arr);

    _experiment->to_set_registrator(_pRegistrator);
    //alg
    _regulator->to_set_koefficients(3);
    _definder->to_set_signal(20);

    _pAxisX->setTitleText("time (sec)");
    _pAxisY->setTitleText("angular displacement  (rad)");
    _pSeries->setName("θ(t)");
    _pSeries_def->setName("θref(t)");
}
void ChartWidget_regulator::_to_run()
{

    _to_reset_chart();

    _pMainChart->update();

    qDebug() << "DONE1\n";


    _experiment->to_set_registrator(_pRegistrator);
    _pRegistrator->to_set_dt_to_plot(_experiment->to_check_time_to_show());
    _experiment->to_run();

    qDebug() << "DONE0\n";
}


void iChartWidgetConfig::_to_handle_changes()
{
    if ( _p_config == nullptr ) { qDebug() << "the _to_handle_changes() ERROR: _p_config == nullptr"; return ; }
    _p_config->insert_or_assign(value::K0, _pk0Edit->text().toDouble());
    _p_config->insert_or_assign(value::K1, _pk1Edit->text().toDouble());

    if (nullptr != _pkpEdit) _p_config->insert_or_assign(value::KP, _pkpEdit->text().toDouble());
    if (nullptr != _pkiEdit) _p_config->insert_or_assign(value::KI, _pkiEdit->text().toDouble());
    if (nullptr != _pkdEdit) _p_config->insert_or_assign(value::KD, _pkdEdit->text().toDouble());


    _p_config->insert_or_assign(value::INPUT, _pInputEdit->text().toDouble());
    /*
    _p_config->insert_or_assign(value::KP, 0.175);
    _p_config->insert_or_assign(value::KI, 0.0190217);
    _p_config->insert_or_assign(value::KD, 1.0626);*/
}

bool iChartWidgetConfig::_is_changed_config()
{
    return true;
}

void iChartWidget::_to_apply_changes()
{
    if ( _p_config == nullptr ) { qDebug() << "the _to_apply_changes() ERROR: _p_config == nullptr"; return ; }
    auto vector_engine = _acs_model->to_check_process()->to_check_parameters();
    vector_engine[DC_engine::LOAD_K_0] = _p_config->at(value::K0);
    vector_engine[DC_engine::LOAD_K_1] = _p_config->at(value::K1);
    auto vector_regulator = vector_engine;
    if ( _acs_model->to_check_regulator() != nullptr )
    try {
        vector_regulator = _acs_model->to_check_regulator()->to_check_parameters();
        vector_regulator[PID_regulator::K_P] = _p_config->at(value::KP);
        vector_regulator[PID_regulator::K_I] = _p_config->at(value::KI);
        vector_regulator[PID_regulator::K_D] = _p_config->at(value::KD);
        _regulator->to_set_all_parameters(vector_regulator);
    } catch ( std::out_of_range & e )
    {
        ;
    }
    if ( _definder == nullptr ) _source->to_receive_input_signal(_p_config->at(value::INPUT));
    else _definder->to_set_signal(_p_config->at(value::INPUT));
    _process->to_set_all_parameters(vector_engine);
}

void iChartWidgetConfig::_to_init_user_input()
{
    _pk0Edit->setText("20");
    _pk1Edit->setText("0");

    if (nullptr != _pkpEdit) _pkpEdit->setText("3");
    if (nullptr != _pkiEdit) _pkiEdit->setText("0");
    if (nullptr != _pkdEdit) _pkdEdit->setText("0");

    _pInputEdit->setText("20");
}
