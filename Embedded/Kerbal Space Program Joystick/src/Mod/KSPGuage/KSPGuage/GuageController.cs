using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using UnityEngine;

namespace KSPGuage
{
    public class GuageController : PartModule
    {
        #region Constants
        private float UPDATE_INTERVAL = 0.75f;
        #endregion

        #region Private Variables
        private float _lastUpdate;
        private int _currentStage;
        private int _partsCount;
        private List<PartResource> _fuelResources;
        private List<PartResource> _monoPropellantResources;
        private List<PartResource> _electricChargeResources;
        private UpdateMessage _updateMsg;
        #endregion

        #region Destructor
        ~GuageController()
        {
            ResetDisplay();
        }
        #endregion

        #region Public Override Methods
        public override void OnAwake()
        {
            _lastUpdate = 0;
            _currentStage = -1;
            _partsCount = -1;

            _updateMsg = new UpdateMessage();

            base.OnAwake();
        }

        public override void OnUpdate()
        {
            if ((Time.time - _lastUpdate) > UPDATE_INTERVAL)
            {
                _lastUpdate = Time.time;

                if (_currentStage != vessel.currentStage || _partsCount != vessel.Parts.Count)
                {
                    _currentStage = vessel.currentStage;
                    _partsCount = vessel.Parts.Count;

                    UpdateResources();
                }

                UpdateDisplay();
            }
        }

        public override void OnInactive()
        {
            ResetDisplay();
            base.OnInactive();
        }
        #endregion

        #region Private Methods
        private void UpdateResources()
        {
            _fuelResources = new List<PartResource>();
            _monoPropellantResources = new List<PartResource>();
            _electricChargeResources = new List<PartResource>();

            foreach (Part part in vessel.Parts.Where(p =>
                p.partInfo.category == PartCategories.FuelTank ||
                p.partInfo.category == PartCategories.Engine ||
                p.partInfo.category == PartCategories.Propulsion ||
                p.partInfo.category == PartCategories.Pods || 
                p.partInfo.title.StartsWith("Z-") ||
                p.partInfo.title.StartsWith("Fuel Cell")))
            {
                foreach (PartResource resource in part.Resources)
                {
                    string resourceType = resource.info.name.ToLower();

                    if (resourceType == "liquidfuel" || resourceType == "solidfuel")
                        _fuelResources.Add(resource);
                    else if (resourceType == "electriccharge")
                        _electricChargeResources.Add(resource);
                    else if (resourceType == "monopropellant")
                        _monoPropellantResources.Add(resource);
                }
            }
        }

        private float GetUtilizationPercentage(ref List<PartResource> resourcesList, bool activeResourcesOnly)
        {
            float utilized = 0;
            float capacity = 0;

            foreach (PartResource resource in resourcesList)
            {
                if ((activeResourcesOnly && resource.amount != resource.maxAmount) || !activeResourcesOnly)
                {
                    utilized += (float)resource.amount;
                    capacity += (float)resource.maxAmount;
                }
            }

            return utilized / capacity;
        }

        private int GetSpeed()
        {
            if (vessel.altitude > 35000)
                return (int)vessel.obt_velocity.magnitude;
            else
                return (int)vessel.srf_velocity.magnitude;
        }

        private int GetAltitude()
        {
            return (int)vessel.altitude;
        }

        private bool GetSAS()
        {
            return vessel.ActionGroups[KSPActionGroup.SAS];
        }

        private bool GetRCS()
        {
            return vessel.ActionGroups[KSPActionGroup.RCS];
        }

        private void UpdateDisplay()
        {
            _updateMsg.SetFuelPercentage(GetUtilizationPercentage(ref _fuelResources, false));
            _updateMsg.SetMonoPropellantPercentage(GetUtilizationPercentage(ref _monoPropellantResources, false));
            _updateMsg.SetElectricChargePercentage(GetUtilizationPercentage(ref _electricChargeResources, false));
            _updateMsg.SetSpeed(GetSpeed());
            _updateMsg.SetAltitude(GetAltitude());
            _updateMsg.SetSAS(GetSAS());
            _updateMsg.SetRCS(GetRCS());

            SerialController.Write(_updateMsg.GetBytes());
        }

        private void ResetDisplay()
        {
            SerialController.Write(new ResetMessage().GetBytes());
        }
        #endregion
    }
}
