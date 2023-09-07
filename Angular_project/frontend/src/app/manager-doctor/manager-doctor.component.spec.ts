import { ComponentFixture, TestBed } from '@angular/core/testing';

import { ManagerDoctorComponent } from './manager-doctor.component';

describe('ManagerDoctorComponent', () => {
  let component: ManagerDoctorComponent;
  let fixture: ComponentFixture<ManagerDoctorComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ ManagerDoctorComponent ]
    })
    .compileComponents();

    fixture = TestBed.createComponent(ManagerDoctorComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
