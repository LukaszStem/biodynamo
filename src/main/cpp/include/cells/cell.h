#ifndef CELLS_CELL_H_
#define CELLS_CELL_H_

#include <array>
#include <string>
#include <list>
#include <vector>
#include <memory>

#include "color.h"
#include "sim_state_serializable.h"
#include "local_biology/soma_element.h"
#include "local_biology/neurite_element.h"
#include "cells/cell_module.h"

namespace cx3d {

namespace simulation {
class ECM;  // todo replace with include once porting has been finished and remove include in cc file
}  // namespace simulation

namespace cells {

/**
 * Class <code>Cell</code> implements the cell at biological level. Every cell is characterized
 * by a unique cellId, cellType (cell state), <code>LyonCellCycle</code> (cell cycle) and are eventually
 * organized in a cell lineage tree (<code>CellLinNode</code>).
 * This class contains the genome (for now a list of <code>Gene</code>), a list of <code>GeneSubstance</code>
 * (seen as the product of the genes in the Gene vector), and is characterized by a cell type (defined by the
 * relative concentrations of the GeneSubstances.
 */
class Cell : public SimStateSerializable, public std::enable_shared_from_this<Cell> {
 public:
  /** defines types for the NeuroML export*/
  enum NeuroMLType {
    kInhibitory,
    kExcitatatory
  };

  static void setECM(const std::shared_ptr<simulation::ECM>& ecm) {
    ecm_ = ecm;
  }

  static std::shared_ptr<Cell> create() {
    auto cell = std::shared_ptr<Cell>(new Cell());
    cell->init();
    return cell;
  }

  /**
   * Generate <code>Cell</code>. and registers the <code>Cell</code> to <code>ECM<</code>.
   * Every cell is identified by a unique cellID number.
   */
  Cell();

  virtual ~Cell();

  virtual StringBuilder& simStateToJson(StringBuilder& sb) const override;

  virtual std::string toString() const;

  virtual bool equalTo(const std::shared_ptr<Cell>& other) const;

  /**
   * Run Cell: run <code>Gene</code>, run <code>LyonCellCycle</code>, run Conditions, run EnergyProduction.
   * We move one step further in the simulation by running the <code>Gene</code>, <code>GeneSubstances</code>,
   * the <code>LyonCellCycle</code>, EnergyProduction and than we test conditions with ConditionTester.
   */
  virtual void run();

  // *************************************************************************************
  // *      METHODS FOR DIVISION                                                         *
  // *************************************************************************************

  /**
   * Divide the cell. Of the two daughter cells, one is this one (but smaller, with half GeneSubstances etc.),
   * and the other one is instantiated de novo and is returned. Both cells have more or less the same volume,
   * the axis of division is random.
   * @return the other daughter cell.
   */
  virtual std::shared_ptr<Cell> divide();

  /**
   * Divide the cell. Of the two daughter cells, one is this one (but smaller, with half GeneSubstances etc.),
   * and the other one is instantiated de novo and is returned. The axis of division is random.
   * @param volumeRatio the ratio (Volume daughter 1)/(Volume daughter 2). 1.0 gives equal cells.
   * @return the second daughter cell.
   */
  virtual std::shared_ptr<Cell> divide(double volume_ratio);

  /**
   * @param axis specifies direction of division
   */
  virtual std::shared_ptr<Cell> divide(const std::array<double, 3>& axis);

  /**
   * Divide the cell. Of the two daughter cells, one is this one (but smaller, with half GeneSubstances etc.),
   * and the other one is instantiated de novo and is returned. The axis of division is random.
   * @param volumeRatio the ratio (Volume daughter 1)/(Volume daughter 2). 1.0 gives equal cells.
   * @param axis specifies direction of division
   * @return the second daughter cell
   */
  virtual std::shared_ptr<Cell> divide(double volume_ratio, const std::array<double, 3>& axis);

  /**
   * Divide mother cell in two daughter cells by coping <code>Cell</code>, <code>SomaElement</code>,
   * <code>PhysicalSphere</code>, list of <code>CellModules</code>.
   * <code>CellSubstances</code> are dispatched in the two cells.
   * The <code>CellClock</code>  and cell lineage, if present, are also copied..
   * When mother cell divides, by definition:
   * 1) the mother cell becomes the 1st daughter cell
   * 2) the new cell becomes the 2nd daughter cell and inherits a equal or bigger volume than the 1st
   *    daughter cell, which means that this cell will eventually inherit more differentiating factors
   *    and will be recorded in the left side of the lineage tree.
   *
   * @return the second daughter cell
   */
  virtual std::shared_ptr<Cell> divide(double volume_ratio, double phi, double theta);

  // *************************************************************************************
  // *      METHODS FOR CELL MODULES                                                     *
  // *************************************************************************************

  /**
   * Adds a <code>CellModule</code> that will be run at each time step.
   * @param m
   */
  virtual void addCellModule(const std::shared_ptr<cx3d::cells::CellModule>& m);
  /**
   * Removes a particular <code>CellModule</code> from this <code>Cell</code>.
   * It will therefore not be run anymore.
   * @param m
   */
  virtual void removeCellModule(const std::shared_ptr<cx3d::cells::CellModule>& m);

  /** Removes all the <code>CellModule</code> in this <code>Cell</code>.*/
  virtual void cleanAllCellModules();

  /**
   * Sets the color for all the <code>PhysicalObjects</code> associated with the
   * <code>CellElements</code> of this Cell..
   * @param color
   */
  virtual void setColorForAllPhysicalObjects(cx3d::Color color);

  // *************************************************************************************
  // *      GETTERS & SETTERS                                                            *
  // *************************************************************************************

  /** Currently, there are two types of cells : Inhibitory_cells and Excitatory_cells.*/
  virtual void setNeuroMLType(NeuroMLType neuro_ml_type);

  /** Currently, there are two types of cells :  <code>Inhibitory_cells</code> and  <code>Excitatory_cells</code>.*/
  virtual NeuroMLType getNeuroMLType() const;

  /** Returns the cell type. This is just a convenient way to store some property for the cell.
   * Should not be confused with NeuroMLType.
   */
  virtual std::string getType() const;

  /** Sets the cell type. This is just a convenient way to store some property for the cell.
   * Should not be confused with NeuroMLType.
   */
  virtual void setType(const std::string& type);

  virtual std::shared_ptr<local_biology::SomaElement> getSomaElement() const;

  virtual void setSomaElement(const std::shared_ptr<local_biology::SomaElement>& soma);

  virtual int getID() const;

  /** Returns the list of all the CellModules.*/
  virtual std::vector<std::shared_ptr<cx3d::cells::CellModule> > getCellModules() const;

  /**
   * @return a <code>Vector</code> containing all the <code>NeuriteElement</code>s of this cell.
   */
  virtual std::list<std::shared_ptr<local_biology::NeuriteElement>> getNeuriteElements() const;

 private:
  Cell(const Cell&) = delete;
  Cell& operator=(const Cell&) = delete;

  /* Unique identification for this Cell instance. */
  int id_ = 0;

  /* Counter to uniquely identify every cell. */
  static int id_counter_;

  /* Reference to the ECM. */
  static std::shared_ptr<simulation::ECM> ecm_;

  /* List of all cell modules that are run at each time step*/
  std::vector<std::shared_ptr<CellModule>> cell_modules_;  //todo change to vector once porting has been finished

  /* List of the SomaElements belonging to the cell */
  std::shared_ptr<local_biology::SomaElement> soma_;

  /* List of the first Neurite of all Nurites belonging to the cell */
  std::list<std::shared_ptr<local_biology::NeuriteElement>> neurite_root_list_;  // TODO: not working yet

  /* The electrophsiology type of this cell */
  NeuroMLType neuro_ml_type_ = NeuroMLType::kExcitatatory;

  /** Some convenient way to store properties of  for cells.
   * Should not be confused with neuroMLType. */
  std::string type_ = "";

  void init();
};

}  // cells
}  // cx3d

#endif  // CELLS_CELL_H_
